/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 24, 2016
 **/

#include <icarus/config.h>
#include <boost/filesystem/operations.hpp>
#include <icarus/exceptions.h>
#include <icarus/log.h>

icarus::config::database::database()
{ }

icarus::config::database::database(const std::string &driver, unsigned int pool_size)
	: _driver(driver), _pool_size(pool_size)
{ }

icarus::config::database::database(const database &database)
	: _data(database._data), _driver(database._driver), _pool_size(database._pool_size)
{ }

const std::string &icarus::config::database::driver() const
{
	return this->_driver;
}

icarus::config::database &icarus::config::database::driver(const std::string &driver)
{
	this->_driver = driver;
	return *this;
}

unsigned int icarus::config::database::pool_size() const
{
	return this->_pool_size;
}

icarus::config::database &icarus::config::database::pool_size(unsigned int pool_size)
{
	this->_pool_size = pool_size;
	return *this;
}

void icarus::config::database::add(const std::string &param, const std::string &value)
{
	this->_data.emplace(param, value);
}

const std::string icarus::config::database::str() const
{
	std::string result(this->_driver);
	result += "://";
	unsigned int i = 0;
	for (const std::pair<std::string, std::string> &pair : this->_data)
	{
		if (i++ > 0)
			result += " ";
		result += pair.first + "=" + pair.second;
	}
	return result;
}

boost::optional<icarus::config::database> icarus::config::databases::operator[](const std::string &name)
{
	const icarus::config::databases::iterator &it = this->_data.find(name);
	if (it == this->_data.end())
		return boost::optional<icarus::config::database>();
	else
		return boost::optional<icarus::config::database>(it->second);
}

icarus::config::databases::iterator icarus::config::databases::begin()
{
	return this->_data.begin();
}

icarus::config::databases::iterator icarus::config::databases::end()
{
	return this->_data.end();
}

icarus::config::databases::const_iterator icarus::config::databases::begin() const
{
	return this->_data.begin();
}

icarus::config::databases::const_iterator icarus::config::databases::end() const
{
	return this->_data.end();
}

icarus::config::databases::reverse_iterator icarus::config::databases::rbegin()
{
	return this->_data.rbegin();
}

icarus::config::databases::reverse_iterator icarus::config::databases::rend()
{
	return this->_data.rend();
}

icarus::config::databases::const_iterator icarus::config::databases::cbegin()
{
	return this->_data.cbegin();
}

icarus::config::databases::const_iterator icarus::config::databases::cend()
{
	return this->_data.cend();
}

icarus::config::databases::const_reverse_iterator icarus::config::databases::crbegin()
{
	return this->_data.crbegin();
}

icarus::config::databases::const_reverse_iterator icarus::config::databases::crend()
{
	return this->_data.crend();
}

size_t icarus::config::databases::size()
{
	return this->_data.size();
}

icarus::config::database &icarus::config::databases::add(const std::string &name, const std::string &driver, const unsigned int poolSize)
{
	std::pair<icarus::config::databases::iterator, bool> it
		= this->_data.emplace(name, icarus::config::database(driver, poolSize));
	return (it.first)->second;
}

icarus::config::session_memcached_server::session_memcached_server(const std::string &address)
	: _address(address)
{ }

const std::string &icarus::config::session_memcached_server::address() const
{
	return this->_address;
}

unsigned int icarus::config::config::threads()
{
	return this->_threads;
}

icarus::config::databases &icarus::config::config::databases()
{
	return this->_databases;
}

const icarus::config::session_memcached_server &icarus::config::session_memcached::operator[](const unsigned int index)
{
	return this->_servers[index];
}

icarus::config::session_memcached_server &icarus::config::session_memcached::add(const std::string &address)
{
	return *this->_servers.emplace(this->_servers.end(), address);
}

icarus::config::session_memcached::iterator icarus::config::session_memcached::begin()
{
	return this->_servers.begin();
}

icarus::config::session_memcached::const_iterator icarus::config::session_memcached::begin() const
{
	return this->_servers.begin();
}

icarus::config::session_memcached::iterator icarus::config::session_memcached::end()
{
	return this->_servers.end();
}

icarus::config::session_memcached::const_iterator icarus::config::session_memcached::end() const
{
	return this->_servers.end();
}

icarus::config::session_memcached::const_iterator icarus::config::session_memcached::cbegin()
{
	return this->_servers.cbegin();
}

icarus::config::session_memcached::const_iterator icarus::config::session_memcached::cend()
{
	return this->_servers.cend();
}

icarus::config::session_memcached::reverse_iterator icarus::config::session_memcached::rbegin()
{
	return this->_servers.rbegin();
}

icarus::config::session_memcached::reverse_iterator icarus::config::session_memcached::rend()
{
	return this->_servers.rend();
}

icarus::config::session_memcached::const_reverse_iterator icarus::config::session_memcached::crbegin()
{
	return this->_servers.crbegin();
}

icarus::config::session_memcached::const_reverse_iterator icarus::config::session_memcached::crend()
{
	return this->_servers.crend();
}

size_t icarus::config::session_memcached::size() const
{
	return this->_servers.size();
}

std::unique_ptr<icarus::config::session_memcached> &icarus::config::session::memcached()
{
	return this->_memcached;
}

icarus::config::session &icarus::config::config::session()
{
	return this->_session;
}

void icarus::config::config::loadFromFile(const std::string &fname)
{
	boost::filesystem::path path(fname);

	if (!boost::filesystem::exists(fname))
		throw icarus::file_not_found(fname);
	if (!boost::filesystem::is_regular(fname))
		throw icarus::not_a_file(fname);

	boost::property_tree::ptree pt;
	boost::property_tree::read_info(fname, pt);

	const boost::optional<boost::property_tree::ptree&> &server = pt.get_child_optional("server");
	if (server)
	{
		this->_threads = server->get<unsigned int>("threads", 10);
	}

	const boost::optional<boost::property_tree::ptree&> &dbs = pt.get_child_optional("databases");
	if (dbs)
	{
		for (boost::property_tree::ptree::value_type &db : dbs.get())
		{
			icarus::config::database &dbref = this->databases().add(
				db.first,
				db.second.get<std::string>("driver", ""),
				db.second.get<unsigned int>("pool_size", 10)
			);
			for (boost::property_tree::ptree::value_type &dbproperty : db.second)
			{
				if ((dbproperty.first != "driver") && (dbproperty.first != "pool_size"))
				{
					dbref.add(dbproperty.first, dbproperty.second.data());
				}
			}
		}
	}

	const boost::optional<boost::property_tree::ptree&> &session = pt.get_child_optional("session");
	if (session)
	{
		const boost::optional<boost::property_tree::ptree&> &memcached = session->get_child_optional("memcached");
		if (memcached)
		{
			this->_session.memcached().reset(new icarus::config::session_memcached());
			for (boost::property_tree::ptree::value_type &server : memcached.get())
			{
				if (server.first == "server")
					this->_session.memcached()->add(server.second.get<std::string>("address"));
			}
		}
	}
}
