/**
 * @author J. Santos <jamillo@gmail.com>
 * @date March 02, 2016
 **/

#ifndef ICARUSFRAMEWORK_MIGRATIONS_MANAGER_H
#define ICARUSFRAMEWORK_MIGRATIONS_MANAGER_H

#include <icarus/migrations/migration.h>

#include <soci/soci.h>
#include <boost/date_time/gregorian/greg_date.hpp>

namespace icarus
{
namespace migrations
{

struct registry
{
	icarus::migrations::version version;

	icarus::migrations::migration_id_t id;

	std::string username;

	boost::gregorian::date installed_on;

	unsigned int execution_time;
};

class manager
{
public:
	typedef typename std::map<std::string, std::unique_ptr<icarus::migrations::group>>::iterator iterator;
	typedef typename std::map<std::string, std::unique_ptr<icarus::migrations::group>>::reverse_iterator reverse_iterator;

	typedef typename std::pair<std::string, std::unique_ptr<icarus::migrations::group>> pair;

	class identification
	{
	public:
		identification();

		identification(const icarus::migrations::manager::iterator &group,
			const icarus::migrations::group::iterator &migration);

		icarus::migrations::manager::iterator group;
		icarus::migrations::group::iterator migration;
	};
protected:
	bool _fetched;

	std::vector<registry> _data;

	std::map<std::string, std::unique_ptr<icarus::migrations::group>> _migrations;

	virtual identification current();

	virtual bool schemaExists() = 0;

	virtual void createSchema() = 0;

	virtual void ensureSchema();

	virtual void fetch() = 0;

	virtual void insert(icarus::migrations::migration &migration) = 0;

	virtual void remove(icarus::migrations::migration &migration) = 0;
public:
	manager();

	virtual void to(const std::string &version, icarus::migrations::migration_id_t id);

	virtual void to(const std::string &version);

	virtual void upgrade();

	virtual void add(icarus::migrations::migration *migration);
};
}
}

#endif //ICARUSFRAMEWORK_MIGRATIONS_MANAGER_H
