/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#include <icarus/http/headers.h>
#include <icarus/general.h>

icarus::http::header_value::header_value()
{ }

icarus::http::header_value::header_value(const std::string &value)
{
	this->emplace(value);
}

boost::optional<icarus::http::header_value &> icarus::http::headers::operator[](const std::string &name)
{
	auto it = this->_list.find(name);
	if (it == this->_list.end())
		return boost::optional<header_value &>();
	else
		return boost::optional<header_value &>(it->second);
}

const std::string &icarus::http::header_value::value()
{
	return (this->_values.empty() ? icarus::emptyString : this->_values[0]);
}

icarus::http::header_value &icarus::http::header_value::value(const std::string &value)
{
	if (this->_values.empty())
		this->emplace(value);
	else
		this->_values[0] = value;
	return *this;
}

icarus::http::header_value &icarus::http::header_value::emplace(const std::string &value)
{
	this->_values.emplace_back(value);
	return *this;
}

icarus::http::header_value::iterator icarus::http::header_value::begin()
{
	return this->_values.begin();
}

icarus::http::header_value::const_iterator icarus::http::header_value::begin() const
{
	return this->_values.begin();
}

icarus::http::header_value::iterator icarus::http::header_value::end()
{
	return this->_values.end();
}

icarus::http::header_value::const_iterator icarus::http::header_value::end() const
{
	return this->_values.end();
}

icarus::http::header_value::const_iterator icarus::http::header_value::cbegin() const
{
	return this->_values.cbegin();
}

icarus::http::header_value::const_iterator icarus::http::header_value::cend() const
{
	return this->_values.cend();
}

icarus::http::header_value::reverse_iterator icarus::http::header_value::rbegin()
{
	return this->_values.rbegin();
}

icarus::http::header_value::reverse_iterator icarus::http::header_value::rend()
{
	return this->_values.rend();
}

icarus::http::header_value::const_reverse_iterator icarus::http::header_value::crbegin() const
{
	return this->_values.crbegin();
}

icarus::http::header_value::const_reverse_iterator icarus::http::header_value::crend() const
{
	return this->_values.crend();
}

icarus::http::headers::headers()
{ }

icarus::http::headers::iterator icarus::http::headers::begin()
{
	return this->_list.begin();
}

icarus::http::headers::const_iterator icarus::http::headers::begin() const
{
	return this->_list.begin();
}

icarus::http::headers::iterator icarus::http::headers::end()
{
	return this->_list.end();
}

icarus::http::headers::const_iterator icarus::http::headers::end() const
{
	return this->_list.end();
}

icarus::http::headers::const_iterator icarus::http::headers::cbegin() const
{
	return this->_list.cbegin();
}

icarus::http::headers::const_iterator icarus::http::headers::cend() const
{
	return this->_list.cend();
}

icarus::http::headers::reverse_iterator icarus::http::headers::rbegin()
{
	return this->_list.rbegin();
}

icarus::http::headers::reverse_iterator icarus::http::headers::rend()
{
	return this->_list.rend();
}

icarus::http::headers::const_reverse_iterator icarus::http::headers::crbegin() const
{
	return this->_list.crbegin();
}

icarus::http::headers::const_reverse_iterator icarus::http::headers::crend() const
{
	return this->_list.crend();
}

void icarus::http::headers::emplace(const std::string &name, const std::string &value)
{
	this->_list.emplace(name, value);
}
