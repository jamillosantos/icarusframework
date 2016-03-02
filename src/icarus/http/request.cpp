/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 12, 2016
 **/

#include <icarus/http/request.h>

icarus::http::request::request()
{ }

icarus::http::request::~request()
{ }

icarus::http::value_list<icarus::http::values_value> &icarus::http::request::headers()
{
	return this->_headers;
}

icarus::http::value_list<icarus::http::values_value> &icarus::http::request::serverVariables()
{
	return this->_serverVariables;
}

std::istream &icarus::http::request::content()
{
	return *this->_content.get();
}

const std::string &icarus::http::request::contentType() const
{
	return this->_contentType;
}

const long long int &icarus::http::request::contentLength() const
{
	return this->_contentLength;
}

icarus::http::cookies &icarus::http::request::cookies()
{
	return this->_cookies;
}

const std::string &icarus::http::request::uri()
{
	return this->_uri;
}

const std::string &icarus::http::request::queryString()
{
	return this->_queryString;
}

const std::string &icarus::http::request::method()
{
	return this->_method;
}