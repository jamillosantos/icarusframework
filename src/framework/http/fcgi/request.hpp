/**
* @author J. Santos <jamillo@gmail.com>
* @date January 13, 2016
**/

#ifndef ICARUSFRAMEWORK_FCGI_REQUEST_HPP
#define ICARUSFRAMEWORK_FCGI_REQUEST_HPP

#include "../request.hpp"
#include "../../exceptions.hpp"
#include "../cookies.hpp"
#include "../../log.hpp"

#include <boost/network/uri.hpp>
#include <boost/log/trivial.hpp>

namespace icarus
{
namespace http
{
namespace fcgi
{
class Request
	: public icarus::http::Request
{
private:
	std::unique_ptr<std::istream> _content;
	Values _serverVariables;
	std::string _contentType;
	long long int _contentLength;
	Cookies _cookies;
public:
	Values &serverVariables()
	{
		return this->_serverVariables;
	}

	virtual void init(const FCGX_Request &request)
	{
		this->_contentLength = -1;

		std::string header, headerName, headerValue;
		size_t fr;
		icarus::log << "Parsing headers:";
		for (char **p = request.envp; *p; ++p)
		{
			header = *p;
			fr = header.find('=');
			if (fr != std::string::npos)
			{
				headerName = header.substr(0, fr);
				headerValue = header.substr(fr + 1, header.length() - fr - 1);
				if (headerName == "HTTP_COOKIE")
				{
					this->_cookies.parse(headerValue);
				}
				else if (headerName.compare(0, 5, "HTTP_") == 0)
				{
					this->_headers.add(
						headerName.substr(5, headerName.length() - 5),
						headerValue
					);
				}
				else
				{
					this->_serverVariables.add(
						headerName,
						headerValue
					);
					if (headerName == "CONTENT_TYPE")
						this->_contentType = headerValue;
					else if (headerName == "CONTENT_LENGTH")
					{
						char *estr;
						this->_contentLength = std::strtol(headerValue.c_str(), &estr, 10);
						if (*estr)
						{
							this->_contentLength = -1;
						}
					}
				}
			}
			else
			{
				icarus::log << "Malformed header: " << header;
			}
		}
		this->_content.reset(new fcgi_istream(request.in));
	}

	template<class T>
	Request &operator>>(T& t)
	{
		(*this->_content) >> t;
		return *this;
	}

	std::istream &content()
	{
		return *this->_content.get();
	}

	const std::string &contentType()
	{
		return this->_contentType;
	}

	long long int contentLength()
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
}

#endif //ICARUSFRAMEWORK_FCGI_REQUEST_HPP
