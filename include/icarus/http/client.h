/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 05, 2016
 **/

#ifndef ICARUSFRAMEWORK_HTTP_CLIENT_H
#define ICARUSFRAMEWORK_HTTP_CLIENT_H

#include <icarus/http/request.h>
#include <icarus/http/response.h>

namespace icarus
{
namespace http
{
class client_context
{
protected:
	icarus::http::irequest &_request;
	icarus::http::response &_response;
public:
	client_context(client_context &clientContext);

	client_context(icarus::http::irequest &req, icarus::http::response &resp);

	virtual ~client_context();

	icarus::http::irequest &request();

	icarus::http::response &response();
};
}
}

#endif //ICARUSFRAMEWORK_HTTP_CLIENT_H
