/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 24, 2016
 **/

#include <icarus/db/databases.h>
#include <icarus/log.h>

icarus::db::databases::databases()
{ }

icarus::db::databases::databases(icarus::config::databases &config)
{
	for (const icarus::config::databases::pair &db : config)
		this->add(db.first, db.second);
}

void icarus::db::databases::add(const std::string &name, const icarus::config::database &database)
{
	soci::connection_pool *pool = new soci::connection_pool(database.pool_size());
	for (size_t i = 0; i < database.pool_size(); ++i)
	{
		soci::session &sql = pool->at(i);
		sql.open(database.str());
	}
	this->_pools.emplace(name, std::unique_ptr<soci::connection_pool>(pool));
}

boost::optional<soci::connection_pool&> icarus::db::databases::operator[](const std::string &name)
{
	auto it = this->_pools.find(name);
	if (it == this->_pools.end())
		return boost::optional<soci::connection_pool&>();
	else
		return boost::optional<soci::connection_pool&>(*it->second);
}

icarus::db::databases::iterator icarus::db::databases::begin()
{
	return this->_pools.begin();
}

icarus::db::databases::iterator icarus::db::databases::end()
{
	return this->_pools.end();
}

icarus::db::databases::const_iterator icarus::db::databases::begin() const
{
	return this->_pools.begin();
}

icarus::db::databases::const_iterator icarus::db::databases::end() const
{
	return this->_pools.end();
}

icarus::db::databases::const_iterator icarus::db::databases::cbegin() const
{
	return this->_pools.cbegin();
}

icarus::db::databases::const_iterator icarus::db::databases::cend() const
{
	return this->_pools.cend();
}

icarus::db::databases::reverse_iterator icarus::db::databases::rbegin()
{
	return this->_pools.rbegin();
}

icarus::db::databases::reverse_iterator icarus::db::databases::rend()
{
	return this->_pools.rend();
}

icarus::db::databases::const_reverse_iterator icarus::db::databases::crbegin() const
{
	return this->_pools.crbegin();
}

icarus::db::databases::const_reverse_iterator icarus::db::databases::crend() const
{
	return this->_pools.crend();
}
