/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 24, 2016
 **/

#include <icarus/db/databases.h>

void icarus::db::databases::add(const std::string &name, icarus::config::database &database)
{
	soci::connection_pool *pool = new soci::connection_pool(database.poolSize());
	this->_pools.emplace(std::make_pair(name, std::unique_ptr<soci::connection_pool>(pool)));
}

const boost::optional<soci::connection_pool> &icarus::db::databases::operator[](const std::string &name)
{
	auto it = this->_pools.find(name);
	if (it == this->_pools.end())
		return boost::optional<soci::connection_pool>();
	else
		return *it->second;
}
