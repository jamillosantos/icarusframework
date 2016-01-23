/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#ifndef ICARUSFRAMEWORK_HTTP_RESPONSE_HPP
#define ICARUSFRAMEWORK_HTTP_RESPONSE_HPP

#include <sstream>
#include "headers.hpp"
#include "statuses.hpp"

namespace icarus
{
namespace http
{

template <class ResponseWriter>
class Response
{
public:
	static std::string endh;
private:
	Status &status;
	std::stringstream stream;

	ValueList<Value> _headers;

	bool _headerSent;
protected:
	std::unique_ptr<ResponseWriter> _responseWriter;

	virtual void flushHeaders()
	{
		(*this->_responseWriter) << "HTTP/1.1 " << this->status.code << " " << this->status.value << endh;
		for (Value &header : this->headers())
			(*this->_responseWriter) << header.name() << ": " << header.value() << endh;
		(*this->_responseWriter) << endh;
		this->_headerSent = true;
	}
public:
	Response()
		: status(statuses::OK), stream(std::stringstream::binary | std::stringstream::in | std::stringstream::out),
		  _headerSent(false)
	{ }

	virtual ~Response()
	{
		this->flush();
	}

	ValueList<Value> &headers()
	{
		return this->_headers;
	}

	virtual void flush()
	{
		if (!this->_headerSent)
			this->flushHeaders();
		(*this->_responseWriter) << this->stream.rdbuf();
	}

	template<typename T>
	Response &operator<<(const T &t)
	{
		this->stream << t;
		return *this;
	}

	Response &operator<<(std::ostream &(*manip)(std::ostream &))
	{
		this->stream << manip;
		return *this;
	}
};

template <typename T>
std::string icarus::http::Response<T>::endh(
	"\r\n");
}
}


#endif //ICARUSFRAMEWORK_HTTP_RESPONSE_HPP
