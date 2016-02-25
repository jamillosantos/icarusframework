#include <stdlib.h>
#include <fstream>

#include <fcgio.h>

#include <boost/core/null_deleter.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/sinks/text_file_backend.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/sinks/text_ostream_backend.hpp>

#include <boost/program_options.hpp>
#include <icarus/multithreadedapplication.hpp>

#include "request.hpp"
#include "response.hpp"
#include "../../log.hpp"
#include "../../routes/data.hpp"
#include "application.hpp"

int main(int argc, char **argv)
{
	std::string
		configFile,
		log,
		logFile;

	boost::program_options::options_description commandLineOnlyOptions("Command Line Options");

	commandLineOnlyOptions.add_options()
		("version,v", "print version string")
		("help,h", "print help message")
		("config,c", boost::program_options::value<std::string>(&configFile)->default_value("config/config"), "configuration file")
		("log,l", boost::program_options::value<std::string>(&log)->default_value("all"), "log configuration (info,warning,debug,trace;all;none)")
		("log-file,c", boost::program_options::value<std::string>(&logFile)->default_value("icarus.log"), "log file");

	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(commandLineOnlyOptions).run(), vm);
	boost::program_options::notify(vm);

	if (vm.count("version"))
	{
		std::cout << "Version: " << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH << "." <<
			VERSION_TWEAK << std::endl;
	}
	else
	{
		// Log initialization
		if (log == "all")
		{
			boost::shared_ptr<boost::log::core> core = boost::log::core::get();

			// Create a backend and attach a couple of streams to it
			boost::shared_ptr<boost::log::sinks::text_ostream_backend> backend =
				boost::make_shared<boost::log::sinks::text_ostream_backend>();
			backend->add_stream(
				boost::shared_ptr<std::ostream>(&std::clog, boost::null_deleter()));
			backend->add_stream(
				boost::shared_ptr<std::ostream>(new std::ofstream(logFile)));

			// Enable auto-flushing after each log record written
			backend->auto_flush(true);

			// Wrap it into the frontend and register in the core.
			// The backend requires synchronization in the frontend.
			typedef boost::log::sinks::synchronous_sink<boost::log::sinks::text_ostream_backend> sink_t;
			boost::shared_ptr<sink_t> sink(new sink_t(backend));
			core->add_sink(sink);
		}

		LOG_INFO("System initializing ...");
		LOG_INFO("PID: " << getpid());
		LOG_INFO("Starting with config file: " << configFile);
		LOG_INFO("Logging '" << log << "' to '" << logFile << "'");
		LOG_INFO("-----------------------");

		icarus::http::fcgi::application app;
		app.config().loadFromFile(configFile);

		LOG_INFO("Config dump:");
		LOG_INFO(" + requestThreads: " << app.config().threads());
		LOG_INFO(" + databases: ");
		for (auto &db : app.config().databases())
		{
			LOG_INFO("\t + " << db.first << " at '" << db.second.string() << "' with " << db.second.poolSize() << " connections");
		}

		icarus::multi_threaded_application mapp(app);
		mapp.run();
	}
	return 0;
}
