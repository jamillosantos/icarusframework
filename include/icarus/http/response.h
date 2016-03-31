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

	icarus::http::headers _headers;

	bool _header_sent;
protected:
	std::ostream *ostream;

	virtual void flush_headers();

	virtual void flush_headers(const icarus::result &result);
public:
	response();

	virtual ~response();

	icarus::http::headers &headers();

	virtual void flush();

	template<typename T>
	icarus::http::response &operator<<(const T &t)
	{
		if (!this->_header_sent)
			this->flush_headers();
		(*this->ostream) << t;
		return *this;
	}

	icarus::http::response & operator<<(const icarus::result &result);

	icarus::http::response & operator<<(icarus::content &content);

	icarus::http::response & operator<<(std::ostream &(*manip)(std::ostream &));
};
}
}


#endif //ICARUSFRAMEWORK_HTTP_RESPONSE_H
