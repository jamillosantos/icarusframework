/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 14, 2016
 **/

#include <icarus/http/cookies.h>

icarus::http::cookies_parser_exception::cookies_parser_exception(std::exception *cause)
		: icarus::exception("Cannot parse the cookies field.", cause)
	{ }

icarus::http::cookies_parser_exception::cookies_parser_exception()
	: icarus::http::cookies_parser_exception(nullptr)
{ }

icarus::http::cookie::cookie()
	: values_value()
{ }

icarus::http::cookie::cookie(const std::string &name, const std::string &value)
	: icarus::http::values_value(name, value)
{ }

icarus::http::cookie::cookie(const std::string &name, const std::string &value, const boost::optional<std::chrono::system_clock::time_point> &expires)
	: icarus::http::values_value(name, value), _expires(expires)
{ }

const boost::optional<std::chrono::system_clock::time_point>& icarus::http::cookie::expires() const
{
	return this->_expires;
}

void icarus::http::cookie::expires(const std::chrono::system_clock::time_point &dt)
{
	this->_expires.reset(dt);
}

void icarus::http::cookies::parse(const std::string &cookies)
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

boost::regex icarus::http::cookies::regex("[ \\t]*([^=]+)=([^;]+)(; *|$)");
