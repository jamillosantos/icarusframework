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
class response
{
public:
	static std::string endh;
private:
	icarus::http::status &status;

	value_list<values_value> _headers;

	bool _header_sent;
protected:
	std::stringstream stream;

	std::ostream *ostream;

	virtual void flushHeaders()
	{
		(*this->ostream) << "HTTP/1.1 " << this->status.code << " " << this->status.value << endh;
		LOG_TRACE("HTTP/1.1 " << this->status.code << " " << this->status.value << endh);
		for (values_value &header : this->headers())
		{
			(*this->ostream) << header.name() << ": " << header.value() << endh;
			LOG_TRACE(header.name() << ": " << header.value() << endh);
		}
		(*this->ostream) << endh;

		this->_header_sent = true;
	}
public:
	response()
		: status(statuses::OK), _header_sent(false),
		  stream(std::stringstream::binary | std::stringstream::in | std::stringstream::out), ostream(nullptr)
	{ }

	virtual ~response()
	{ }

	value_list<values_value> &headers()
	{
		return this->_headers;
	}

	virtual void flush()
	{
		if (!this->_header_sent)
			this->flushHeaders();
		(*this->ostream) << this->stream.rdbuf();
	}

	template<typename T>
	response &operator<<(const T &t)
	{
		this->stream << t;
		return *this;
	}

	response &operator<<(std::ostream &(*manip)(std::ostream &))
	{
		this->stream << manip;
		return *this;
	}
};

std::string icarus::http::response::endh("\r\n");
}
}


#endif //ICARUSFRAMEWORK_HTTP_RESPONSE_HPP
