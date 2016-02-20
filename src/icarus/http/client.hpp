/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 05, 2016
 **/

#ifndef ICARUSFRAMEWORK_CLIENT_HPP
#define ICARUSFRAMEWORK_CLIENT_HPP

#include "request.hpp"
#include "response.hpp"

namespace icarus
{
namespace http
{
class ClientContext
{
private:
	Request &_request;
	Response &_response;
public:
	ClientContext(Request &request, Response &response)
		: _request(request), _response(response)
	{ }

	Request &request()
	{
		return this->_request;
	}

	Response &response()
	{
		return this->_response;
	}
};
}
}

#endif //ICARUSFRAMEWORK_CLIENT_HPP