/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#ifndef ICARUSFRAMEWORK_HTTP_RESPONSE_H
#define ICARUSFRAMEWORK_HTTP_RESPONSE_H


#include <sstream>

#include <icarus/statuses.h>
#include <icarus/result.h>
#include <icarus/log.h>

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

	virtual void flush_headers();

	virtual void flush_headers(icarus::result &result);
public:
	response();

	virtual ~response();

	value_list<values_value> &headers();

	virtual void flush();

	template<typename T>
	icarus::http::response &operator<<(const T &t)
	{
		if (!this->_header_sent)
			this->flush_headers();
		(*this->ostream) << t;
		return *this;
	}

	icarus::http::response &operator<<(icarus::result &result)
	{
		if (!this->_header_sent)
			this->flush_headers(result);
		(*this->ostream) << result.content().rdbuf();
		return *this;
	}

	icarus::http::response &operator<<(std::ostream &(*manip)(std::ostream &))
	{
		if (!this->_header_sent)
			this->flush_headers();
		(*this->ostream) << manip;
		return *this;
	}
};
}
}


#endif //ICARUSFRAMEWORK_HTTP_RESPONSE_H
