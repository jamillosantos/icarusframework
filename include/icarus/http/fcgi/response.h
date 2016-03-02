/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 21, 2016
 **/

#ifndef ICARUSFRAMEWORK_HTTP_FCGI_RESPONSE_HPP
#define ICARUSFRAMEWORK_HTTP_FCGI_RESPONSE_HPP

#include <icarus/http/response.h>
#include <fcgio.h>

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
	response();

	response(response &response);

	virtual ~response() override;

	void init(FCGX_Request &fcgiRequest);
};
}
}
}

#endif //ICARUSFRAMEWORK_HTTP_FCGI_RESPONSE_HPP
