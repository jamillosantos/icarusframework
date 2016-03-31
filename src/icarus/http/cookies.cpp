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

icarus::http::cookie_value::cookie_value()
{ }

icarus::http::cookie_value::cookie_value(const std::string &value)
{ }

icarus::http::cookie_value::cookie_value(const std::string &value, const boost::optional<std::chrono::system_clock::time_point> &expires)
{ }

const boost::optional<std::chrono::system_clock::time_point>& icarus::http::cookie_value::expires() const
{
	return this->_expires;
}

void icarus::http::cookie_value::expires(const std::chrono::system_clock::time_point &dt)
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
		this->emplace(results[1], results[2]);
		start = results[0].second;
	}
}

boost::regex icarus::http::cookies::regex("[ \\t]*([^=]+)=([^;]+)(; *|$)");

const boost::optional<icarus::http::cookie_value&> icarus::http::cookies::operator[](const std::string &name)
{
	icarus::http::cookies::iterator it = this->_list.find(name);
	if (it == this->_list.end())
		return boost::optional<icarus::http::cookie_value&>();
	else
		return boost::optional<icarus::http::cookie_value&>(it->second);
}

void icarus::http::cookies::emplace(const std::string &name, const std::string &value)
{
	auto it = this->_list.emplace(name, value);
	if (!it.second)
		it.first->second = value;
}

icarus::http::cookies::iterator icarus::http::cookies::begin()
{
	this->_list.begin();
}

icarus::http::cookies::iterator icarus::http::cookies::end()
{
	this->_list.end();
}

icarus::http::cookies::const_iterator icarus::http::cookies::cbegin()
{
	this->_list.cbegin();
}

icarus::http::cookies::const_iterator icarus::http::cookies::cend()
{
	this->_list.cend();
}


icarus::http::cookies::reverse_iterator icarus::http::cookies::rbegin()
{
	this->_list.rbegin();
}

icarus::http::cookies::reverse_iterator icarus::http::cookies::rend()
{
	this->_list.rend();
}

icarus::http::cookies::const_reverse_iterator icarus::http::cookies::crbegin()
{
	this->_list.crbegin();
}

icarus::http::cookies::const_reverse_iterator icarus::http::cookies::crend()
{
	this->_list.crend();
}

