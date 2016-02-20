/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 21, 2016
 **/

#ifndef ICARUSFRAMEWORK_HTTP_FCGI_RESPONSE_HPP
#define ICARUSFRAMEWORK_HTTP_FCGI_RESPONSE_HPP

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
	std::streambuf *_rdbuf;
public:
	virtual ~Response()
	{
		this->flush();
		delete this->outStream;
		LOG_TRACE("~fcgi::Response");
	}

	void init(fcgi_streambuf *outStreamBuf)
	{
		this->outStream = new std::ostream(outStreamBuf);
	}
};
}
}
}

#endif //ICARUSFRAMEWORK_HTTP_FCGI_RESPONSE_HPP
