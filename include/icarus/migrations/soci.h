/**
 * @author J. Santos <jamillo@gmail.com>
 * @date March 03, 2016
 **/

#ifndef ICARUSFRAMEWORK_MIGRATIONS_MYSQL_H
#define ICARUSFRAMEWORK_MIGRATIONS_MYSQL_H

#include <icarus/migrations/manager.h>
#include <icarus/migrations/runner.h>

#ifndef MIGRATION__TABLE_NAME
#define MIGRATION__TABLE_NAME "__migrations"
#endif

namespace icarus
{
namespace migrations
{

class migration_soci
	: public icarus::migrations::migration
{
protected:
	soci::session *_session;
public:
	icarus::migrations::migration_soci &session(soci::session &session);
};

class manager_soci
	: public icarus::migrations::manager
{
private:
	boost::optional<soci::session&> _session;

protected:
	virtual bool schemaExists() override;

	virtual void createSchema() override;

	virtual void fetch() override;

	virtual void insert(icarus::migrations::migration &migration);

	virtual void remove(icarus::migrations::migration &migration);

public:
	icarus::migrations::manager_soci &session(soci::session &session);
};

class runner_soci
	: public icarus::migrations::runner
{
public:
	runner_soci(icarus::migrations::manager &manager);

	virtual void run(int argc, char **argv) override;
};
}
}

namespace soci
{
template<>
struct type_conversion<icarus::migrations::registry>
{
	typedef soci::values base_type;

	static void from_base(soci::values &v, soci::indicator, icarus::migrations::registry &registry);

	static void to_base(const icarus::migrations::registry &registry, soci::values &values, soci::indicator &ind);
};
}

#endif //ICARUSFRAMEWORK_MIGRATIONS_MYSQL_H
