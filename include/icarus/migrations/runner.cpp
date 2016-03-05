/**
 * @author J. Santos <jamillo@gmail.com>
 * @date March 04, 2016
 **/

#include "runner.h"

icarus::migrations::manager &icarus::migrations::runner::manager()
{
	return this->_manager;
}

icarus::migrations::runner_options &icarus::migrations::runner::options()
{
	return this->_options;
}

icarus::config::config &icarus::migrations::runner::config()
{
	return this->_config;
}

icarus::migrations::runner::runner(icarus::migrations::manager &manager)
	: _manager(manager)
{ }

void icarus::migrations::runner::run(int argc, char **argv)
{
	std::string
		configFile,
		version;

	icarus::migrations::migration_id_t id = UINT_MAX;

	boost::program_options::options_description commandLineOnlyOptions("Command Line Options");

	commandLineOnlyOptions.add_options()
		("version,v", boost::program_options::value<std::string>(&version)->default_value(this->options().version ? this->options().version->value : ""), "configuration file")
		("id,i", boost::program_options::value<icarus::migrations::migration_id_t>(&id), "configuration file")
		("config,c", boost::program_options::value<std::string>(&configFile)->default_value("config/config"), "configuration file")
		("debug,d", "debug mode on");

	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(commandLineOnlyOptions).run(), vm);
	boost::program_options::notify(vm);

	this->config().loadFromFile(configFile);

	if (version != "")
		this->options().version = version;
	if (id != UINT_MAX)
		this->options().id = id;
	this->options().debug = (vm.count("debug") > 0);
}
