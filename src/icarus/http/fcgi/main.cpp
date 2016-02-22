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
#include "application.hpp"

void init()
{
	boost::shared_ptr<boost::log::core> core = boost::log::core::get();

	// Create a backend and attach a couple of streams to it
	boost::shared_ptr<boost::log::sinks::text_ostream_backend> backend =
		boost::make_shared<boost::log::sinks::text_ostream_backend>();
	backend->add_stream(
		boost::shared_ptr<std::ostream>(&std::clog, boost::null_deleter()));
	backend->add_stream(
		boost::shared_ptr<std::ostream>(new std::ofstream("icarus.log")));

	// Enable auto-flushing after each log record written
	backend->auto_flush(true);

	// Wrap it into the frontend and register in the core.
	// The backend requires synchronization in the frontend.
	typedef boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend> sink_t;
	boost::shared_ptr<sink_t> sink(new sink_t(backend));
	core->add_sink(sink);
}

static void penv(icarus::http::Response &response, const char *const *envp)
{
	response << "<PRE>\n";
	for (; *envp; ++envp)
		response << *envp << "\n";
	response << "</PRE>\n";
}

int main(void)
{
	int count = 0;
	long pid = getpid();

	init();

	LOG_INFO("System initializing ...");
	LOG_INFO("PID: " << getpid());
	LOG_INFO("-----------------------");

	icarus::http::fcgi::Application app;
	app.run();

	return 0;
}
