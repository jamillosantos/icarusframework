/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 05, 2016
 **/

#include <icarus/http/client.h>

icarus::http::client_context::client_context(client_context &clientContext)
	: _request(clientContext._request), _response(clientContext._response)
{ }

icarus::http::client_context::client_context(icarus::http::irequest &req, icarus::http::response &resp)
	: _request(req), _response(resp)
{ }

icarus::http::client_context::~client_context()
{ }

icarus::http::irequest &icarus::http::client_context::request()
{
	return this->_request;
}

icarus::http::response &icarus::http::client_context::response()
{
	return this->_response;
}
