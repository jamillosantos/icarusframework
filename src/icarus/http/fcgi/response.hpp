/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 21, 2016
 **/

#ifndef ICARUSFRAMEWORK_HTTP_FCGI_RESPONSE_HPP
#define ICARUSFRAMEWORK_HTTP_FCGI_RESPONSE_HPP

#include <fcgio.h>
#include "../response.hpp"

namespace icarus
{
namespace http
{
namespace fcgi
{
class Response
	: public icarus::http::Response
{
private:
	fcgi_streambuf *_streambuf;
public:
	Response()
		: _streambuf(nullptr), icarus::http::Response::Response()
	{ }

	Response(Response &response)
		: _streambuf(response._streambuf)
	{
		this->outStream = response.outStream;
	}

	virtual ~Response() override
	{
		LOG_TRACE("~Response");
		this->flush();

		if (this->outStream)
			delete this->outStream;

		if (this->_streambuf)
			delete this->_streambuf;
	}

	void init(FCGX_Request &fcgiRequest)
	{
		this->_streambuf = new fcgi_streambuf(fcgiRequest.out);
		this->outStream = new std::ostream(this->_streambuf);
	}
};
}
}
}

#endif //ICARUSFRAMEWORK_HTTP_FCGI_RESPONSE_HPP
