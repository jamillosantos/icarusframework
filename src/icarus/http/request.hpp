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
public:
	Request()
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
};
}
}

#endif //ICARUSFRAMEWORK_REQUEST_HPP
