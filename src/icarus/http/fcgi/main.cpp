#include <stdlib.h>
#include <fstream>

#include <fcgio.h>

#include <boost/core/null_deleter.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>

#include "request.hpp"
#include "response.hpp"
#include "../../log.hpp"
#include "../../routes/data.hpp"

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

template <typename T>
static void penv(icarus::http::Response<T> &response, const char * const * envp)
{
	response << "<PRE>\n";
	for ( ; *envp; ++envp)
		response << *envp << "\n";
	response << "</PRE>\n";
}

int main (void)
{
	int count = 0;
	long pid = getpid();

	init();

	LOG_INFO("System initializing ...");

	FCGX_Request fcgiRequest;

	FCGX_Init();
	FCGX_InitRequest(&fcgiRequest, 0, 0);

	LOG_INFO("Ok.");
	LOG_INFO("Waiting client...");
	while (FCGX_Accept_r(&fcgiRequest) == 0)
	{
		LOG_INFO("Accepted client.");

		fcgi_streambuf cout_fcgi_streambuf(fcgiRequest.out);
		fcgi_streambuf cerr_fcgi_streambuf(fcgiRequest.err);

		icarus::http::fcgi::Request request;
		request.init(fcgiRequest);
		icarus::http::fcgi::Response response;
		response.init(&cout_fcgi_streambuf);

		response << "<html>\n"
			"<title>echo-cpp</title>\n"
			"<H1>echo-cpp</H1>\n"
			"<H4>PID: " << pid << "</H4>\n"
			"<H4>URI: " << request.uri() << "</H4>\n"
			"<H4>Query string: " << request.queryString() << "</H4>\n"
			"<H4>Request Number: " << ++count << "</H4>\n";

		response << "<H4>Headers</H4>\n";
		response << "<dl>";
		for (icarus::http::Value &h : request.headers())
		{
			response << "<dt>" << h.name() << "</dt><dd>" << h.value() << "</dd>";
		}
		response << "</dl>";
		response << "<H4>Server Variables</H4>\n";
		response << "<dl>";
		for (icarus::http::Value &variable : request.serverVariables())
		{
			response << "<dt>" << variable.name() << "</dt><dd>" << variable.value() << "</dd>";
		}
		response << "</dl>";
		response << "<H4>Cookies</H4>\n";
		response << "<dl>";
		for (std::pair<const std::string, icarus::http::Cookie> &cookie : request.cookies())
		{
			response << "<dt>" << cookie.first << "</dt><dd>" << cookie.second.value() << "</dd>";
		}
		response << "</dl>";

		response << "<H4>Standard Input</H4>\n";
		response << "<dl><dt>Content length</dt><dd>" << request.contentLength() << "</dd></dl>" << std::endl;
		response << "<pre>";
		for (std::string line; std::getline(request.content(), line);)
		{
			response << line << std::endl;
		}
		response << "</pre></body></html>";

		response.flush();

		LOG_INFO("Waiting client...");
	}

	return 0;
}
