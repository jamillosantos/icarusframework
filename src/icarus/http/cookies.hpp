/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 14, 2016
 **/

#ifndef ICARUSFRAMEWORK_HTTP_COOKIES_HPP
#define ICARUSFRAMEWORK_HTTP_COOKIES_HPP

#include <boost/regex.hpp>
#include <boost/concept_check.hpp>
#include <boost/log/trivial.hpp>
#include <chrono>
#include <iomanip>
#include <boost/optional/optional.hpp>
#include "headers.hpp"

#include "../log.hpp"
#include "../exceptions.hpp"

namespace icarus
{
namespace http
{
class CookiesParseException
	: public icarus::exceptions::Exception
{
public:
	CookiesParseException(std::exception *cause)
		: Exception("Cannot parse the cookies field.", cause)
	{ }

	CookiesParseException()
		: CookiesParseException(nullptr)
	{ }
};

class Cookie
	: public Value
{
private:
	boost::optional<std::chrono::system_clock::time_point> _expires;
public:
	Cookie()
		: Value()
	{ }

	Cookie(const std::string &name, const std::string &value)
		: Value(name, value)
	{ }

	Cookie(const std::string &name, const std::string &value, const boost::optional<std::chrono::system_clock::time_point> &expires)
		: Value(name, value), _expires(expires)
	{ }

	const boost::optional<std::chrono::system_clock::time_point>& expires() const
	{
		return this->_expires;
	}

	void expires(const std::chrono::system_clock::time_point &dateTime)
	{
		this->_expires.reset(dateTime);
	}
};

class Cookies
	: public ValuesHash<Cookie>
{
private:
	static boost::regex regex;
public:
	void parse(std::string cookies)
	{
		boost::smatch results;

		std::string::const_iterator start = cookies.begin();
		std::string::const_iterator end = cookies.end();

		while (boost::regex_search(start, end, results, regex))
		{
			this->set(results[1], results[2]);
			start = results[0].second;
		}
	}
};

boost::regex Cookies::regex("[ \\t]*([^=]+)=([^;]+)(; *|$)");
}
}

#endif //ICARUSFRAMEWORK_HTTP_COOKIES_HPP
