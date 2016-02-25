/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 24, 2016
 **/

#ifndef ICARUSFRAMEWORK_DB_DATABASES_HPP
#define ICARUSFRAMEWORK_DB_DATABASES_HPP

#include <memory>
#include <map>

#include <soci/connection-pool.h>
#include <icarus/config.hpp>

namespace icarus
{
namespace db
{
class databases
{
private:
	std::map<std::string, std::unique_ptr<soci::connection_pool>> _pools;
public:
	void add(const std::string &name, icarus::config::Database &database)
	{
		soci::connection_pool *pool = new soci::connection_pool(database.poolSize());
		this->_pools.emplace(std::make_pair(name, pool));
	}

	const boost::optional<soci::connection_pool> &operator[](const std::string &name)
	{
		auto it = this->_pools.find(name);
		if (it == this->_pools.end())
			return nullptr;
		else
			return *it->second;
	}
};
}
}

#endif //ICARUSFRAMEWORK_DB_DATABASES_HPP
