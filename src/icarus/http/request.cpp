/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 12, 2016
 **/

#include <icarus/http/request.h>

#include <json/reader.h>

icarus::http::request::request()
{ }

icarus::http::request::~request()
{ }

icarus::http::headers &icarus::http::request::headers()
{
	return this->_headers;
}

icarus::http::headers &icarus::http::request::serverVariables()
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

icarus::http::query_string_values &icarus::http::request::params()
{
	return this->_params;
}

const std::string &icarus::http::request::method()
{
	return this->_method;
}

const Json::Value &icarus::http::request::as_json()
{
	if (!this->_json)
	{
		this->_json.reset(new Json::Value());
		Json::Reader reader;
		reader.parse(this->content(), *this->_json, false);
	}
	return *this->_json;
}
