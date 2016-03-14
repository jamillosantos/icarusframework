/**
 * @author J. Santos <jamillo@gmail.com>
 * @date March 03, 2016
 **/

#include <icarus/migrations/soci.h>
#include <icarus/db/databases.h>
#include <icarus/log.h>

icarus::migrations::migration_soci &icarus::migrations::migration_soci::session(soci::session &session)
{
	this->_session = &session;
	return *this;
}

bool icarus::migrations::manager_soci::schemaExists()
{
	int count;
	const std::string table_name(MIGRATION__TABLE_NAME);
	(*this->_session) << "select count(*) from information_schema.TABLES as t WHERE (t.TABLE_SCHEMA = database()) AND (t.TABLE_NAME = :table_name)",
		soci::use(table_name),
		soci::into(count);
	return (count > 0);
}

void icarus::migrations::manager_soci::createSchema()
{
	LOG_INFO("Creating migrations schema.");
	(*this->_session)
		<< "create table " << MIGRATION__TABLE_NAME << "("
		<< "	version varchar(30), id int unsigned, username varchar(100), installed_on datetime,"
		<< "	execution_time int unsigned, primary key(version, id)"
		<< ") engine=InnoDB";
}

void icarus::migrations::manager_soci::fetch()
{
	if (this->_fetched)
		return;
	LOG_INFO("Fetching data.")
	soci::rowset<icarus::migrations::registry> migrations = ((*this->_session).prepare << "select * from __migrations order by version, id");
	for (const icarus::migrations::registry &r : migrations)
		this->_data.emplace_back(r);
	LOG_INFO(this->_data.size() << " migration registries found on the database");
	this->_fetched = true;
}

void icarus::migrations::manager_soci::insert(icarus::migrations::migration &migration)
{
	char un[60];
	size_t un_len = getlogin_r(un, sizeof(un));
	std::string username(un, un_len);
	(*this->_session) << "insert into " << MIGRATION__TABLE_NAME << "(version, id, username, installed_on, execution_time) values(:version, :id, :username, NOW(), 0)",
		soci::use(migration.version(), "version"), soci::use(migration.id(), "id"), soci::use(username, "username");
}

void icarus::migrations::manager_soci::remove(icarus::migrations::migration &migration)
{
	(*this->_session) << "delete from " << MIGRATION__TABLE_NAME << " where version = :version and id = :id",
		soci::use(migration.version()), soci::use(migration.id());
}

icarus::migrations::manager_soci& icarus::migrations::manager_soci::session(soci::session &session)
{
	this->_session = session;

	for (icarus::migrations::manager::iterator g_it = this->_migrations.begin(); g_it != this->_migrations.end(); ++g_it)
	{
		icarus::migrations::migration_soci *ms;
		for (icarus::migrations::group::iterator m_it = g_it->second->begin(); m_it != g_it->second->end(); ++m_it)
		{
			if ((ms = dynamic_cast<icarus::migrations::migration_soci*>(m_it->get())))
				ms->session(session);
		}
	}
	return *this;
}

icarus::migrations::runner_soci::runner_soci(icarus::migrations::manager &manager)
	: runner(manager)
{ }

void icarus::migrations::runner_soci::run(int argc, char **argv)
{
	LOG_INFO("testing!");

	icarus::migrations::runner::run(argc, argv);

	icarus::db::databases databases(this->config());

	LOG_INFO(this->config().databases()["default"].string());

	icarus::migrations::manager_soci *m = dynamic_cast<icarus::migrations::manager_soci*>(&this->manager());
	if (m)
	{
		boost::optional<soci::connection_pool> pool = databases[(this->options().dbid.empty()) ? "default" : this->options().dbid];
		if (pool)
		{
			soci::session session(*pool);

			if (this->options().debug)
				session.set_log_stream(&std::cerr);

			m->session(session);

			if (this->options().version && this->options().id)
				this->manager().to(this->options().version->value, *(this->options().id));
			else if (this->options().version)
				this->manager().to(this->options().version->value);
			else
				this->manager().upgrade();
		}
		else
			// TODO
			throw icarus::exception("The database identifier was not found.");
	}
	else
		// TODO
		throw icarus::exception("Invalid manager type.");
}

void soci::type_conversion<icarus::migrations::registry>::from_base(soci::values &v, soci::indicator, icarus::migrations::registry &registry)
{
	registry.version = v.get<std::string>("version");
	registry.id = v.get<unsigned int>("id");
	registry.username = v.get<std::string>("username");
	registry.installed_on = v.get<boost::gregorian::date>("installed_on");
	registry.execution_time = v.get<unsigned int>("execution_time");
}

void soci::type_conversion<icarus::migrations::registry>::to_base(const icarus::migrations::registry &registry, soci::values &values, soci::indicator &ind)
{
	// TODO
	ind = soci::i_ok;
}
