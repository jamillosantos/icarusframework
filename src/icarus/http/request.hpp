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
class Request
{
protected:
	ValueList<Value> _headers;
	ValueList<Value> _serverVariables;

	std::string _contentType;
	long long int _contentLength;

	Cookies _cookies;

	std::unique_ptr<std::istream> _content;

	std::string _uri;
	std::string _queryString;
	std::string _method;
public:
	Request()
	{ }

	virtual ~Request()
	{ }

	ValueList<Value> &headers()
	{
		return this->_headers;
	}

	ValueList<Value> &serverVariables()
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

	Cookies &cookies()
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
