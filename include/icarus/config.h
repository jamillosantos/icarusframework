/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 24, 2016
 **/

#ifndef ICARUSFRAMEWORK_CONFIG_H
#define ICARUSFRAMEWORK_CONFIG_H

#include <string>
#include <map>

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
	std::string _string;
	std::string _password;
	unsigned int _poolSize;
public:
	database(const std::string &string, const std::string &password, unsigned int poolSize);

	database(const std::string &string, const std::string &password);

	database(const icarus::config::database &database);

	database();

	const std::string &string();

	icarus::config::database &string(const std::string string);

	const std::string &password();

	icarus::config::database &password(const std::string password);

	unsigned int poolSize();

	database &poolSize(unsigned int poolSize);
};

class databases
{
public:
	typedef typename std::map<std::string, icarus::config::database>::iterator iterator;
private:
	std::map<std::string, icarus::config::database> _data;
public:
	icarus::config::database &operator[](const std::string &name);

	iterator begin();

	iterator end();

	void add(const std::string &name, const std::string &string, const std::string password, const unsigned int poolSize);
};

class config
{
private:
	unsigned int _threads;
	icarus::config::databases _databases;
public:
	unsigned int threads();

	icarus::config::databases &databases();

	void loadFromFile(const std::string &fname);
};
}
}

#endif //ICARUSFRAMEWORK_CONFIG_H