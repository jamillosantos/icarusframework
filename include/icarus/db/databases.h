/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 24, 2016
 **/

#ifndef ICARUSFRAMEWORK_DB_DATABASES_H
#define ICARUSFRAMEWORK_DB_DATABASES_H

#include <memory>
#include <map>

#include <soci/soci.h>

#include <icarus/config.h>
#include <icarus/nullable.h>

namespace icarus
{
namespace db
{
class databases
{
private:
	std::map<std::string, std::unique_ptr<soci::connection_pool>> _pools;
public:
	databases();

	databases(icarus::config::config &config);

	void add(const std::string &name, icarus::config::database &database);

	icarus::nullable<soci::connection_pool> operator[](const std::string &name);
};
}
}

#endif //ICARUSFRAMEWORK_DB_DATABASES_H
