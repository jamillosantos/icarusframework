/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 24, 2016
 **/

#ifndef ICARUSFRAMEWORK_CONFIG_HPP
#define ICARUSFRAMEWORK_CONFIG_HPP

#include <string>
#include <map>

#include <boost/program_options.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>

namespace icarus
{
namespace config
{
class Database
{
private:
	std::string _string;
	std::string _password;
	unsigned int _poolSize;
public:
	Database(const std::string &string, const std::string &password, unsigned int poolSize)
		: _string(string), _password(password), _poolSize(poolSize)
	{ }

	Database(const std::string &string, const std::string &password)
		: _password(password), _poolSize(10)
	{ }

	Database(const Database &database)
		: _string(database._string), _password(database._password), _poolSize(database._poolSize)
	{ }

	Database()
	{ }

	const std::string &string()
	{
		return this->_string;
	}

	Database &string(const std::string string)
	{
		this->_string = string;
		return *this;
	}

	const std::string &password()
	{
		return this->_password;
	}

	Database &password(const std::string password)
	{
		this->_password = password;
		return *this;
	}

	unsigned int poolSize()
	{
		return this->_poolSize;
	}

	Database &poolSize(unsigned int poolSize)
	{
		this->_poolSize = poolSize;
		return *this;
	}
};

class Databases
{
private:
	std::map<std::string, Database> _data;
public:
	Database &operator[](const std::string &name)
	{
		return this->_data.at(name);
	}

	std::map<std::string, icarus::config::Database>::iterator begin()
	{
		return this->_data.begin();
	}

	std::map<std::string, icarus::config::Database>::iterator end()
	{
		return this->_data.end();
	}

	void add(const std::string &name, const std::string &string, const std::string password, const unsigned int poolSize)
	{
		this->_data.emplace(std::make_pair(name, Database(string, password, poolSize)));
	}
};

class config
{
private:
	unsigned int _threads;
	Databases _databases;
public:
	unsigned int threads()
	{
		return this->_threads;
	}

	Databases &databases()
	{
		return this->_databases;
	}

	void loadFromFile(const std::string &fname)
	{
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
				this->databases().add(
					db.first,
					db.second.get<std::string>("string", ""),
					db.second.get<std::string>("password", ""),
					db.second.get<unsigned int>("pool_size", 10)
				);
			}
		}
	}
};
}
}

#endif //ICARUSFRAMEWORK_CONFIG_HPP
