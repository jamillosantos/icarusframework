/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 24, 2016
 **/

#ifndef ICARUSFRAMEWORK_CONFIG_H
#define ICARUSFRAMEWORK_CONFIG_H

#include <string>
#include <map>
#include <vector>

#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>

namespace icarus
{
namespace config
{
class database
{
private:
	std::map<std::string, std::string> _data;

	std::string _driver;
	unsigned int _pool_size;
public:
	database();

	database(const std::string &driver, unsigned int pool_size);

	database(const icarus::config::database &database);

	const std::string &driver();

	icarus::config::database &driver(const std::string &driver);

	unsigned int pool_size();

	icarus::config::database &pool_size(unsigned int pool_size);

	void add(const std::string &param, const std::string &value);

	const std::string str();
};

class databases
{
public:
	typedef typename std::map<std::string, icarus::config::database>::iterator iterator;
	typedef typename std::map<std::string, icarus::config::database>::reverse_iterator reverse_iterator;

	typedef typename std::map<std::string, icarus::config::database>::const_iterator const_iterator;
	typedef typename std::map<std::string, icarus::config::database>::const_reverse_iterator const_reverse_iterator;
private:
	std::map<std::string, icarus::config::database> _data;
public:
	boost::optional<icarus::config::database> operator[](const std::string &name);

	iterator begin();
	iterator end();

	reverse_iterator rbegin();
	reverse_iterator rend();

	const_iterator cbegin();
	const_iterator cend();

	const_reverse_iterator crbegin();
	const_reverse_iterator crend();

	size_t size();

	icarus::config::database &add(const std::string &name, const std::string &driver, const unsigned int poolSize);
};

class session_memcached_server
{
private:
	std::string _address;
public:
	session_memcached_server(const std::string &address);

	const std::string &address() const;
};

class session_memcached
{
public:
	typedef std::vector<session_memcached_server> list;

	typedef list::iterator iterator;
	typedef list::const_iterator const_iterator;
	typedef list::reverse_iterator reverse_iterator;
	typedef list::const_reverse_iterator const_reverse_iterator;
private:
	list _servers;
public:
	const session_memcached_server& operator[](const unsigned int index);

	session_memcached_server &add(const std::string &address);

	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;

	const_iterator cbegin();
	const_iterator cend();

	reverse_iterator rbegin();
	reverse_iterator rend();

	const_reverse_iterator crbegin();
	const_reverse_iterator crend();

	size_t size() const;
};

class session
{
private:
	std::unique_ptr<session_memcached> _memcached;
public:
	std::unique_ptr<session_memcached> &memcached();
};

class config
{
private:
	unsigned int _threads;
	icarus::config::databases _databases;
	icarus::config::session _session;
public:
	unsigned int threads();

	icarus::config::databases &databases();

	icarus::config::session &session();

	void loadFromFile(const std::string &fname);
};
}
}

#endif //ICARUSFRAMEWORK_CONFIG_H