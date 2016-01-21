#include <stdlib.h>
#include <fstream>
#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
extern char ** environ;
#endif
#include "fcgio.h"
#include "fcgi_config.h"  // HAVE_IOSTREAM_WITHASSIGN_STREAMBUF
#include "request.hpp"
#include "../../log.hpp"

#include <boost/core/null_deleter.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
// #include <boost/log/expressions.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>
// #include <boost/log/utility/setup/common_attributes.hpp>
// #include <boost/log/sources/severity_logger.hpp>
// #include <boost/log/sources/record_ostream.hpp>

void init()
{
	/*boost::log::add_file_log(
		boost::log::keywords::file_name = "icarus_%N.log",                                        *//*< file name pattern >*//*
		boost::log::keywords::rotation_size = 10 * 1024 * 1024,                                   *//*< rotate files every 10 MiB... >*//*
		boost::log::keywords::time_based_rotation = boost::log::sinks::file::rotation_at_time_point(0, 0, 0), *//*< ...or at midnight >*//*
		boost::log::keywords::format = "[%TimeStamp%]: %Message%"                                 *//*< log record format >*//*
	);

	boost::log::core::get()->set_filter(
		boost::log::trivial::severity >= boost::log::trivial::info
	);*/
	//boost::log::add_file_log("icarus.log");

	boost::shared_ptr< boost::log::core > core = boost::log::core::get();

	// Create a backend and attach a couple of streams to it
	boost::shared_ptr< boost::log::sinks::text_ostream_backend > backend =
		boost::make_shared< boost::log::sinks::text_ostream_backend >();
	backend->add_stream(
		boost::shared_ptr< std::ostream >(&std::clog, boost::null_deleter()));
	backend->add_stream(
		boost::shared_ptr< std::ostream >(new std::ofstream("icarus.log")));

	// Enable auto-flushing after each log record written
	backend->auto_flush(true);

	// Wrap it into the frontend and register in the core.
	// The backend requires synchronization in the frontend.
	typedef boost::log::sinks::synchronous_sink< boost::log::sinks::text_ostream_backend > sink_t;
	boost::shared_ptr< sink_t > sink(new sink_t(backend));
	core->add_sink(sink);
}


// Maximum number of bytes allowed to be read from stdin
static const unsigned long STDIN_MAX = 1000000;

static void penv(const char * const * envp)
{
	std::cout << "<PRE>\n";
	for ( ; *envp; ++envp)
	{
		std::cout << *envp << "\n";
	}
	std::cout << "</PRE>\n";
}

static long gstdin(FCGX_Request * request, char ** content)
{
	char * clenstr = FCGX_GetParam("CONTENT_LENGTH", request->envp);
	unsigned long clen = STDIN_MAX;

	if (clenstr)
	{
		clen = strtol(clenstr, &clenstr, 10);
		if (*clenstr)
		{
			BOOST_LOG_TRIVIAL(error) << "Can't parse \"CONTENT_LENGTH="
				<< FCGX_GetParam("CONTENT_LENGTH", request->envp)
				<< std::endl;
			clen = STDIN_MAX;
		}

		// *always* put a cap on the amount of data that will be read
		if (clen > STDIN_MAX) clen = STDIN_MAX;

		*content = new char[clen];

		std::cin.read(*content, clen);
		clen = std::cin.gcount();
	}
	else
	{
		// *never* read stdin when CONTENT_LENGTH is missing or unparsable
		*content = 0;
		clen = 0;
	}

	// Chew up any remaining stdin - this shouldn't be necessary
	// but is because mod_fastcgi doesn't handle it correctly.

	// ignore() doesn't set the eof bit in some versions of glibc++
	// so use gcount() instead of eof()...
	do std::cin.ignore(1024); while (std::cin.gcount() == 1024);

	return clen;
}

int main (void)
{
	int count = 0;
	long pid = getpid();

	init();

	icarus::log << "System initializing ...";

	std::streambuf * cin_streambuf  = std::cin.rdbuf();
	std::streambuf * cout_streambuf = std::cout.rdbuf();
	std::streambuf * cerr_streambuf = std::cerr.rdbuf();

	FCGX_Request request;

	FCGX_Init();
	FCGX_InitRequest(&request, 0, 0);

	BOOST_LOG_TRIVIAL(info) << "Ok." << std::endl;
	while (FCGX_Accept_r(&request) == 0)
	{
		// Note that the default bufsize (0) will cause the use of iostream
		// methods that require positioning (such as peek(), seek(),
		// unget() and putback()) to fail (in favour of more efficient IO).
		fcgi_streambuf cout_fcgi_streambuf(request.out);
		fcgi_streambuf cerr_fcgi_streambuf(request.err);

#if HAVE_IOSTREAM_WITHASSIGN_STREAMBUF
		std::cin  = &cin_fcgi_streambuf;
        std::cout = &cout_fcgi_streambuf;
        std::cerr = &cerr_fcgi_streambuf;
#else
		std::cout.rdbuf(&cout_fcgi_streambuf);
		std::cerr.rdbuf(&cerr_fcgi_streambuf);
#endif

		std::cout << "Content-type: text/html\r\n"
			"\r\n"
			"<TITLE>echo-cpp</TITLE>\n"
			"<H1>echo-cpp</H1>\n"
			"<H4>PID: " << pid << "</H4>\n"
			"<H4>Request Number: " << ++count << "</H4>\n";

		std::cout << "<H4>Headers</H4>\n";
		penv(request.envp);
		icarus::http::fcgi::Request r;
		r.init(request);
		std::cout << "<dl>";
		for (icarus::http::Value &h : r.headers())
		{
			std::cout << "<dt>" << h.name << "</dt><dd>" << h.value << "</dd>";
		}
		std::cout << "</dl>";
		std::cout << "<H4>Server Variables</H4>\n";
		std::cout << "<dl>";
		for (icarus::http::Value &variable : r.serverVariables())
		{
			std::cout << "<dt>" << variable.name << "</dt><dd>" << variable.value << "</dd>";
		}
		std::cout << "</dl>";

		std::cout << "<H4>Process/Initial Environment</H4>\n";
		penv(environ);

		std::cout << "<H4>Standard Input</H4>\n";
		std::cout << "<dl><dt>Content length</dt><dd>" << r.contentLength() << "</dd></dl>" << std::endl;
		std::cout << "<pre>";

		for (std::string line; std::getline(r.content(), line);)
		{
			std::cout << line << std::endl;
		}
		std::cout << "</pre></body></html>";

		// If the output streambufs had non-zero bufsizes and
		// were constructed outside of the accept loop (i.e.
		// their destructor won't be called here), they would
		// have to be flushed here.
	}

#if HAVE_IOSTREAM_WITHASSIGN_STREAMBUF
    std::cout = cout_streambuf;
    std::cerr = cerr_streambuf;
#else
	std::cout.rdbuf(cout_streambuf);
	std::cerr.rdbuf(cerr_streambuf);
#endif

	return 0;
}