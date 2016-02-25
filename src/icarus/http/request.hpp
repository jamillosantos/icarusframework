/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 12, 2016
 **/

#ifndef ICARUSFRAMEWORK_REQUEST_HPP
#define ICARUSFRAMEWORK_REQUEST_HPP

#include "headers.hpp"
#include "cookies.hpp"

namespace icarus
{
namespace http
{
class request
{
protected:
	value_list<values_value> _headers;
	value_list<values_value> _serverVariables;

	std::string _contentType;
	long long int _contentLength;

	icarus::http::cookies _cookies;

	std::unique_ptr<std::istream> _content;

	std::string _uri;
	std::string _queryString;
	std::string _method;
public:
	request()
	{ }

	virtual ~request()
	{ }

	value_list<values_value> &headers()
	{
		return this->_headers;
	}

	value_list<values_value> &serverVariables()
	{
		return this->_serverVariables;
	}

	std::istream &content()
	{
		return *this->_content.get();
	}

	const std::string &contentType() const
	{
		return this->_contentType;
	}

	const long long int &contentLength() const
	{
		return this->_contentLength;
	}

	icarus::http::cookies &cookies()
	{
		return this->_cookies;
	}

	const std::string &uri()
	{
		return this->_uri;
	}

	const std::string &queryString()
	{
		return this->_queryString;
	}

	const std::string &method()
	{
		return this->_method;
	}
};
}
}

#endif //ICARUSFRAMEWORK_REQUEST_HPP
