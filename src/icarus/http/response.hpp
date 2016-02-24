/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#ifndef ICARUSFRAMEWORK_HTTP_RESPONSE_HPP
#define ICARUSFRAMEWORK_HTTP_RESPONSE_HPP

#include <sstream>
#include "headers.hpp"
#include "statuses.hpp"
#include <icarus/log.hpp>

namespace icarus
{
namespace http
{
class Response
{
public:
	static std::string endh;
private:
	Status &status;

	ValueList<Value> _headers;

	bool _headerSent;
protected:
	std::stringstream stream;

	std::ostream *outStream;

	virtual void flushHeaders()
	{
		(*this->outStream) << "HTTP/1.1 " << this->status.code << " " << this->status.value << endh;
		LOG_TRACE("HTTP/1.1 " << this->status.code << " " << this->status.value << endh);
		for (Value &header : this->headers())
		{
			(*this->outStream) << header.name() << ": " << header.value() << endh;
			LOG_TRACE(header.name() << ": " << header.value() << endh);
		}
		(*this->outStream) << endh;

		this->_headerSent = true;
	}
public:
	Response()
		: status(statuses::OK), _headerSent(false),
		  stream(std::stringstream::binary | std::stringstream::in | std::stringstream::out), outStream(nullptr)
	{ }

	virtual ~Response()
	{ }

	ValueList<Value> &headers()
	{
		return this->_headers;
	}

	virtual void flush()
	{
		if (!this->_headerSent)
			this->flushHeaders();
		(*this->outStream) << this->stream.rdbuf();
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

std::string icarus::http::Response::endh("\r\n");
}
}


#endif //ICARUSFRAMEWORK_HTTP_RESPONSE_HPP
