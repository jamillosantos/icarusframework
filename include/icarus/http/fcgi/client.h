/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 22, 2016
 **/

#ifndef ICARUSFRAMEWORK_HTTP_FCGI_CLIENT_H
#define ICARUSFRAMEWORK_HTTP_FCGI_CLIENT_H

#include <icarus/http/client.h>

#include <fcgiapp.h>

#include <icarus/http/client.h>
#include <icarus/http/fcgi/request.h>
#include <icarus/http/fcgi/response.h>

namespace icarus
{
namespace http
{
namespace fcgi
{
class client_context
	: public icarus::http::client_context
{
private:
	FCGX_Request *_fcgxRequest;

	fcgi::request *_fcgiRequest;
	fcgi::response *_fcgiResponse;
public:
	client_context(FCGX_Request *fcgiRequest);
	virtual ~client_context() override;

	void init();
};
}
}
}

#endif //ICARUSFRAMEWORK_HTTP_FCGI_CLIENT_H
