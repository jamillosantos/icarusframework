/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#ifndef ICARUSFRAMEWORK_HTTP_RESPONSE_HPP
#define ICARUSFRAMEWORK_HTTP_RESPONSE_HPP

#include <sstream>

#include <icarus/http/headers.hpp>
#include <icarus/statuses.hpp>
#include <icarus/log.hpp>
#include "../result.hpp"

namespace icarus
{
namespace http
{
class response
{
public:
	static std::string endh;
private:
	icarus::status &status;

	value_list<values_value> _headers;

	bool _header_sent;
protected:
	std::ostream *ostream;

	virtual void flush_headers()
	{
		(*this->ostream) << "HTTP/1.1 " << this->status.code << " " << this->status.value << endh;
		for (values_value &header : this->headers())
		{
			(*this->ostream) << header.name() << ": " << header.value() << endh;
		}
		(*this->ostream) << endh;

		this->_header_sent = true;
	}

	virtual void flush_headers(icarus::result &result)
	{
		(*this->ostream) << "HTTP/1.1 " << this->status.code << " " << this->status.value << endh;
		for (values_value &header : result.headers())
		{
			(*this->ostream) << header.name() << ": " << header.value() << endh;
		}
		(*this->ostream) << endh;

		this->_header_sent = true;
	}
public:
	response()
		: status(statuses::OK), _header_sent(false), ostream(nullptr)
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
			this->flush_headers();
		this->ostream->flush();
	}

	template<typename T>
	response &operator<<(const T &t)
	{
		if (!this->_header_sent)
			this->flush_headers();
		(*this->ostream) << t;
		return *this;
	}

	response &operator<<(icarus::result &result)
	{
		if (!this->_header_sent)
			this->flush_headers(result);
		(*this->ostream) << result.content().rdbuf();
		return *this;
	}

	response &operator<<(std::ostream &(*manip)(std::ostream &))
	{
		if (!this->_header_sent)
			this->flush_headers();
		(*this->ostream) << manip;
		return *this;
	}
};

std::string icarus::http::response::endh("\r\n");
}
}


#endif //ICARUSFRAMEWORK_HTTP_RESPONSE_HPP
