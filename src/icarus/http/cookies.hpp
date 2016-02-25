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
class cookies_parser_exception
	: public icarus::exceptions::exception
{
public:
	cookies_parser_exception(std::exception *cause)
		: exception("Cannot parse the cookies field.", cause)
	{ }

	cookies_parser_exception()
		: cookies_parser_exception(nullptr)
	{ }
};

class cookie
	: public values_value
{
private:
	boost::optional<std::chrono::system_clock::time_point> _expires;
public:
	cookie()
		: values_value()
	{ }

	cookie(const std::string &name, const std::string &value)
		: values_value(name, value)
	{ }

	cookie(const std::string &name, const std::string &value, const boost::optional<std::chrono::system_clock::time_point> &expires)
		: values_value(name, value), _expires(expires)
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

class cookies
	: public value_hash<cookie>
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

boost::regex cookies::regex("[ \\t]*([^=]+)=([^;]+)(; *|$)");
}
}

#endif //ICARUSFRAMEWORK_HTTP_COOKIES_HPP
