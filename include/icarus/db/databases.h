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

namespace icarus
{
namespace db
{
class databases
{
public:
	typedef std::map<std::string, std::unique_ptr<soci::connection_pool>> list;
	typedef std::pair<const std::string, std::unique_ptr<soci::connection_pool>> pair;

	typedef list::iterator iterator;
	typedef list::const_iterator const_iterator;
	typedef list::reverse_iterator reverse_iterator;
	typedef list::const_reverse_iterator const_reverse_iterator;
private:
	list _pools;
public:
	databases();

	databases(icarus::config::databases &config);

	void add(const std::string &name, const icarus::config::database &database);

	iterator begin();
	iterator end();

	const_iterator begin() const;
	const_iterator end() const;

	const_iterator cbegin() const;
	const_iterator cend() const;

	reverse_iterator rbegin();
	reverse_iterator rend();

	const_reverse_iterator crbegin() const;
	const_reverse_iterator crend() const;

	boost::optional<soci::connection_pool&> operator[](const std::string &name);
};
}
}

#endif //ICARUSFRAMEWORK_DB_DATABASES_H
