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
class response
	: public icarus::http::response
{
private:
	fcgi_streambuf *_streambuf;
public:
	response()
		: icarus::http::response::response(), _streambuf(nullptr)
	{ }

	response(response &response)
		: _streambuf(response._streambuf)
	{
		this->ostream = response.ostream;
	}

	virtual ~response() override
	{
		LOG_TRACE("~Response");
		this->flush();
		LOG_TRACE("flushed");

		if (this->ostream)
			delete this->ostream;

		if (this->_streambuf)
			delete this->_streambuf;
	}

	void init(FCGX_Request &fcgiRequest)
	{
		this->_streambuf = new fcgi_streambuf(fcgiRequest.out);
		this->ostream = new std::ostream(this->_streambuf);
	}
};
}
}
}

#endif //ICARUSFRAMEWORK_HTTP_FCGI_RESPONSE_HPP
