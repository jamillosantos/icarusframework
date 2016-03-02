/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 24, 2016
 **/

#include <icarus/config.h>

icarus::config::database::database(const std::string &string, const std::string &password, unsigned int poolSize)
	: _string(string), _password(password), _poolSize(poolSize)
{ }

icarus::config::database::database(const std::string &string, const std::string &password)
	: _password(password), _poolSize(10)
{ }

icarus::config::database::database(const database &database)
	: _string(database._string), _password(database._password), _poolSize(database._poolSize)
{ }

icarus::config::database::database()
{ }

const std::string &icarus::config::database::string()
{
	return this->_string;
}

icarus::config::database &icarus::config::database::string(const std::string string)
{
	this->_string = string;
	return *this;
}

const std::string &icarus::config::database::password()
{
	return this->_password;
}

icarus::config::database &icarus::config::database::password(const std::string password)
{
	this->_password = password;
	return *this;
}

unsigned int icarus::config::database::poolSize()
{
	return this->_poolSize;
}

icarus::config::database &icarus::config::database::poolSize(unsigned int poolSize)
{
	this->_poolSize = poolSize;
	return *this;
}

icarus::config::database &icarus::config::databases::operator[](const std::string &name)
{
	return this->_data.at(name);
}

std::map<std::string, icarus::config::database>::iterator icarus::config::databases::begin()
{
	return this->_data.begin();
}

std::map<std::string, icarus::config::database>::iterator icarus::config::databases::end()
{
	return this->_data.end();
}

void icarus::config::databases::add(const std::string &name, const std::string &string, const std::string password, const unsigned int poolSize)
{
	this->_data.emplace(std::make_pair(name, icarus::config::database(string, password, poolSize)));
}

unsigned int icarus::config::config::threads()
{
	return this->_threads;
}

icarus::config::databases &icarus::config::config::databases()
{
	return this->_databases;
}

void icarus::config::config::loadFromFile(const std::string &fname)
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
