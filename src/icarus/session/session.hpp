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
typedef unsigned long long session_id_t;

class session
{
private:
	std::chrono::system_clock::time_point _last_used;
	session_id_t _id;
public:
	session(session_id_t id)
		: _id(id)
	{
		this->renew();
	}

	const session_id_t id()
	{
		return this->_id;
	}

	const std::chrono::system_clock::time_point &last_used()
	{
		return this->_last_used;
	}

	void renew()
	{
		this->_last_used = std::chrono::system_clock::now();
	}

	virtual std::string get(const std::string &name) = 0;

	virtual std::string set(const std::string &name, const std::string &value) = 0;

	virtual std::string as_string(const std::string &name)
	{
		return this->get(name);
	}

	virtual session& as_string(const std::string &name, const std::string &value)
	{
		this->set(name, value);
		return this*;
	}

	template<typename T>
	void set(const std::string &name, const T &value)
	{
		this->set(name, std::to_string(value));
	}

	virtual int8_t as_int8(const std::string &name)
	{
		return boost::lexical_cast<int8_t>(this->get(name));
	}

	virtual uint8_t as_uint8(const std::string &name)
	{
		return boost::lexical_cast<uint8_t>(this->get(name));
	}

	virtual short as_short(const std::string &name)
	{
		return boost::lexical_cast<short>(this->get(name));
	}

	virtual unsigned short as_ushort(const std::string &name)
	{
		return boost::lexical_cast<unsigned short>(this->get(name));
	}

	virtual int as_int(const std::string &name)
	{
		return boost::lexical_cast<int>(this->get(name));
	}

	virtual unsigned int as_uint(const std::string &name)
	{
		return boost::lexical_cast<unsigned int>(this->get(name));
	}

	virtual long as_long(const std::string &name)
	{
		return boost::lexical_cast<long>(this->get(name));
	}

	virtual unsigned long as_ulong(const std::string &name)
	{
		return boost::lexical_cast<unsigned long>(this->get(name));
	}

	virtual long long as_long_long(const std::string &name)
	{
		return boost::lexical_cast<long long>(this->get(name));
	}

	virtual unsigned long long as_ulong_long(const std::string &name)
	{
		return boost::lexical_cast<unsigned long long>(this->get(name));
	}

	virtual float as_float(const std::string &name)
	{
		return boost::lexical_cast<float>(this->get(name));
	}

	virtual double as_double(const std::string &name)
	{
		return boost::lexical_cast<double>(this->get(name));
	}

	virtual long double as_long_double(const std::string &name)
	{
		return boost::lexical_cast<long double>(this->get(name));
	}
};
}
}

#endif //ICARUSFRAMEWORK_SESSION_HPP
