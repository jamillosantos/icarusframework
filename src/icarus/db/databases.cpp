/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 24, 2016
 **/

#include <icarus/db/databases.h>

icarus::db::databases::databases()
{ }

icarus::db::databases::databases(icarus::config::config &config)
{
	for (std::pair<std::string, icarus::config::database> db : config.databases())
		this->add(db.first, db.second);
}

void icarus::db::databases::add(const std::string &name, icarus::config::database &database)
{
	soci::connection_pool *pool = new soci::connection_pool(database.poolSize());
	for (size_t i = 0; i < database.poolSize(); ++i)
	{
		soci::session &sql = pool->at(i);
		sql.open(database.string());
	}
	this->_pools.emplace(std::make_pair(name, std::unique_ptr<soci::connection_pool>(pool)));
}

icarus::nullable<soci::connection_pool> icarus::db::databases::operator[](const std::string &name)
{
	auto it = this->_pools.find(name);
	if (it == this->_pools.end())
		return icarus::nullable<soci::connection_pool>();
	else
		return icarus::nullable<soci::connection_pool>(*it->second);
}
