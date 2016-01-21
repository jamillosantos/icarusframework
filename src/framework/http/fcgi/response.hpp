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
class ResponseWriter
{
private:
	std::ostream *stream;
public:
	ResponseWriter(fcgi_streambuf *outStreamBuf)
	{
		this->stream = new std::ostream(outStreamBuf);
	}

	~ResponseWriter()
	{
		LOG_TRACE("~ResponseWriter()")
		delete this->stream;
	}

	template<class T>
	ResponseWriter &operator<<(const T &t)
	{
		(*this->stream) << t;
		return *this;
	}

	ResponseWriter &operator<<(std::ostream &(*manip)(std::ostream &))
	{
		(*this->stream) << manip;
		return *this;
	}

	ResponseWriter &operator<<(ResponseWriter &(*manip)(ResponseWriter &))
	{
		return manip(*this);
	}
};

class Response
	: public icarus::http::Response<ResponseWriter>
{
public:
	virtual ~Response()
	{
		LOG_TRACE("~fcgi::Response");
	}

	void init(fcgi_streambuf *outStreamBuf)
	{
		this->_responseWriter.reset(new fcgi::ResponseWriter(outStreamBuf));
	}
};

}
}
}

#endif //ICARUSFRAMEWORK_HTTP_FCGI_RESPONSE_HPP
