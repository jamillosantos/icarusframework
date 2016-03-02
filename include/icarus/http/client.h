/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 05, 2016
 **/

#ifndef ICARUSFRAMEWORK_CLIENT_HPP
#define ICARUSFRAMEWORK_CLIENT_HPP

#include <icarus/http/request.h>
#include <icarus/http/response.h>

namespace icarus
{
namespace http
{
class client_context
{
protected:
	icarus::http::request &_request;
	icarus::http::response &_response;
public:
	client_context(client_context &clientContext);

	client_context(icarus::http::request &req, icarus::http::response &resp);

	virtual ~client_context();

	icarus::http::request &request();

	icarus::http::response &response();

	void process();
};
}
}

#endif //ICARUSFRAMEWORK_CLIENT_HPP
