/**
 * @author J. Santos <jamillo@gmail.com>
 * @date November 11, 2015
 **/

#include <icarus/action.h>

icarus::action::action(const action &action)
	: _method(action._method), _url(action._url)
{ }

icarus::action::action(std::string url)
	: _method("GET"), _url(url)
{ }

icarus::action::action(std::string method, std::string url)
	: _method(method), _url(url)
{ }

const std::string &icarus::action::method() const
{
	return this->_method;
}

icarus::action &icarus::action::method(std::string method)
{
	this->_method = method;
	return *this;
}

const std::string &icarus::action::url() const
{
	return this->_url;
}

icarus::action &icarus::action::url(std::string url)
{
	this->_url = url;
	return *this;
}
