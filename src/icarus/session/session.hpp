/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 20, 2016
 **/

#ifndef ICARUSFRAMEWORK_SESSION_HPP
#define ICARUSFRAMEWORK_SESSION_HPP

#include <string>
#include <boost/lexical_cast.hpp>
#include <chrono>

namespace icarus
{
namespace session
{
class Session
{
private:
	std::chrono::system_clock::time_point _lastUsed;
public:
	Session()
	{
		this->renew();
	}

	const std::chrono::system_clock::time_point &lastUsed()
	{
		return this->_lastUsed;
	}

	void renew()
	{
		this->_lastUsed = std::chrono::system_clock::now();
	}

	virtual std::string get(const std::string &name) = 0;

	virtual std::string set(const std::string &name, const std::string &value) = 0;

	virtual std::string asString(const std::string &name)
	{
		return this->get(name);
	}

	virtual Session& asString(const std::string &name, const std::string &value)
	{
		this->set(name, value);
		return this*;
	}

	template<typename T>
	void set(const std::string &name, const T &value)
	{
		this->set(name, std::to_string(value));
	}

	virtual int8_t asInt8(const std::string &name)
	{
		return boost::lexical_cast<int8_t>(this->get(name));
	}

	virtual uint8_t asUInt8(const std::string &name)
	{
		return boost::lexical_cast<uint8_t>(this->get(name));
	}

	virtual short asShort(const std::string &name)
	{
		return boost::lexical_cast<short>(this->get(name));
	}

	virtual unsigned short asUShort(const std::string &name)
	{
		return boost::lexical_cast<unsigned short>(this->get(name));
	}

	virtual int asInt(const std::string &name)
	{
		return boost::lexical_cast<int>(this->get(name));
	}

	virtual unsigned int asUInt(const std::string &name)
	{
		return boost::lexical_cast<unsigned int>(this->get(name));
	}

	virtual long asLong(const std::string &name)
	{
		return boost::lexical_cast<long>(this->get(name));
	}

	virtual unsigned long asULong(const std::string &name)
	{
		return boost::lexical_cast<unsigned long>(this->get(name));
	}

	virtual long long asLongLong(const std::string &name)
	{
		return boost::lexical_cast<long long>(this->get(name));
	}

	virtual unsigned long long asULongLong(const std::string &name)
	{
		return boost::lexical_cast<unsigned long long>(this->get(name));
	}

	virtual float asFloat(const std::string &name)
	{
		return boost::lexical_cast<float>(this->get(name));
	}

	virtual double asDouble(const std::string &name)
	{
		return boost::lexical_cast<double>(this->get(name));
	}

	virtual long double asDouble(const std::string &name)
	{
		return boost::lexical_cast<long double>(this->get(name));
	}
};
}
}

#endif //ICARUSFRAMEWORK_SESSION_HPP
