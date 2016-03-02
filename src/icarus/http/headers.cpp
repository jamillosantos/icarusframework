/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#include <icarus/http/headers.h>

#include <icarus/log.h>

icarus::http::values_value::values_value()
{ }

icarus::http::values_value::values_value(const std::string &name, const std::string &value)
	: _name(name), _value(value)
{ }

const std::string &icarus::http::values_value::name()
{
	return this->_name;
}

void icarus::http::values_value::name(const std::string &name)
{
	this->_name = name;
}

const std::string &icarus::http::values_value::value()
{
	return this->_value;
}

void icarus::http::values_value::value(const std::string &value)
{
	this->_value = value;
}
