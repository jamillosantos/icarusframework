/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 20, 2016
 **/

#ifndef ICARUSFRAMEWORK_SESSION_HPP
#define ICARUSFRAMEWORK_SESSION_HPP

#include <boost/lexical_cast.hpp>
#include <string>
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
	session(session_id_t id);

	const session_id_t id();

	const std::chrono::system_clock::time_point &last_used();

	void renew();

	virtual std::string get(const std::string &name);

	virtual std::string set(const std::string &name, const std::string &value);

	virtual std::string as_string(const std::string &name);

	virtual session& as_string(const std::string &name, const std::string &value);

	template<typename T>
	void set(const std::string &name, const T &value);

	virtual int8_t as_int8(const std::string &name);

	virtual uint8_t as_uint8(const std::string &name);

	virtual short as_short(const std::string &name);

	virtual unsigned short as_ushort(const std::string &name);

	virtual int as_int(const std::string &name);

	virtual unsigned int as_uint(const std::string &name);

	virtual long as_long(const std::string &name);

	virtual unsigned long as_ulong(const std::string &name);

	virtual long long as_long_long(const std::string &name);

	virtual unsigned long long as_ulong_long(const std::string &name);

	virtual float as_float(const std::string &name);

	virtual double as_double(const std::string &name);

	virtual long double as_long_double(const std::string &name);
};
}
}

#endif //ICARUSFRAMEWORK_SESSION_HPP
