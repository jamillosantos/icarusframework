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

icarus::http::cookie_value::cookie_value(const std::string &value, const boost::optional<icarus::http::cookie_expiration_t> &expires)
{ }

const std::string icarus::http::cookie_value::value() const
{
	return this->_value;
}

icarus::http::cookie_value &icarus::http::cookie_value::value(const std::string &value)
{
	this->_value = value;
	return *this;
}

const boost::optional<icarus::http::cookie_expiration_t>& icarus::http::cookie_value::expires() const
{
	return this->_expires;
}

void icarus::http::cookie_value::expires(const icarus::http::cookie_expiration_t &date_time)
{
	this->_expires.reset(date_time);
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

void icarus::http::cookies::set(const std::string &name, const std::string &value)
{
	auto it = this->_list.emplace(name, value);
	if (!it.second)
		it.first->second.value(value);
}

void icarus::http::cookies::set(const std::string &name, const std::string &value,
	const icarus::http::cookie_expiration_t &expiration)
{
	auto it = this->_list.emplace(name, value);
	if (!it.second)
		it.first->second.value(value);
	it.first->second.expires(expiration);
}

icarus::http::cookies::iterator icarus::http::cookies::begin()
{
	return this->_list.begin();
}

icarus::http::cookies::iterator icarus::http::cookies::end()
{
	return this->_list.end();
}

icarus::http::cookies::const_iterator icarus::http::cookies::begin() const
{
	return this->_list.begin();
}

icarus::http::cookies::const_iterator icarus::http::cookies::end() const
{
	return this->_list.end();
}

icarus::http::cookies::const_iterator icarus::http::cookies::cbegin()
{
	return this->_list.cbegin();
}

icarus::http::cookies::const_iterator icarus::http::cookies::cend()
{
	return this->_list.cend();
}

icarus::http::cookies::reverse_iterator icarus::http::cookies::rbegin()
{
	return this->_list.rbegin();
}

icarus::http::cookies::reverse_iterator icarus::http::cookies::rend()
{
	return this->_list.rend();
}

icarus::http::cookies::const_reverse_iterator icarus::http::cookies::crbegin()
{
	return this->_list.crbegin();
}

icarus::http::cookies::const_reverse_iterator icarus::http::cookies::crend()
{
	return this->_list.crend();
}

size_t icarus::http::cookies::size() const
{
	return this->_list.size();
}
