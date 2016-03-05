/**
 * @author J. Santos <jamillo@gmail.com>
 * @date March 04, 2016
 **/

#ifndef ICARUSFRAMEWORK_MIGRATIONS_RUNNER_H
#define ICARUSFRAMEWORK_MIGRATIONS_RUNNER_H

#include <boost/optional.hpp>

#include <icarus/migrations/manager.h>
#include <icarus/migrations/migration.h>
#include <icarus/config.h>

namespace icarus
{
namespace migrations
{

struct runner_options
{
	boost::optional<icarus::migrations::version> version;
	boost::optional<icarus::migrations::migration_id_t> id;
	std::string dbid;
	bool debug;
};

class runner
{
private:
	icarus::migrations::manager &_manager;

	icarus::migrations::runner_options _options;

	icarus::config::config _config;
protected:
	virtual icarus::migrations::manager &manager();

	virtual icarus::migrations::runner_options &options();

	virtual icarus::config::config &config();
public:
	runner(icarus::migrations::manager &manager);

	virtual void run(int argc, char **argv);
};
}
}

#endif //ICARUSFRAMEWORK_MIGRATIONS_RUNNER_H
