/**
* @author J. Santos <jamillo@gmail.com>
* @date January 13, 2016
**/

#ifndef ICARUSFRAMEWORK_FCGI_REQUEST_HPP
#define ICARUSFRAMEWORK_FCGI_REQUEST_HPP

#include "../request.hpp"
#include "../../exceptions.hpp"
#include "icarus/http/cookies.hpp"
#include "../../log.hpp"

#include <boost/network/uri.hpp>
#include <boost/log/trivial.hpp>

namespace icarus
{
namespace http
{
namespace fcgi
{
class request
	: public icarus::http::request
{
public:
	virtual void init(const FCGX_Request &request)
	{
		this->_contentLength = -1;

		std::string header, headerName, headerValue;
		size_t fr;
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
					else if (headerName == "DOCUMENT_URI")
					{
						this->_uri = headerValue;
					}
					else if (headerName == "QUERY_STRING")
					{
						this->_queryString = headerValue;
					}
					else if (headerName == "REQUEST_METHOD")
					{
						this->_method = headerValue;
					}
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
				LOG_ERROR("Malformed header: " << header);
			}
		}
		this->_content.reset(new fcgi_istream(request.in));
	}

	template<class T>
	request &operator>>(T& t)
	{
		(*this->_content) >> t;
		return *this;
	}
};
}
}
}

#endif //ICARUSFRAMEWORK_FCGI_REQUEST_HPP
