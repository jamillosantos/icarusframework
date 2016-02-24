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
protected:
	bool _valid;

	Request &_request;
	Response &_response;
public:
	ClientContext(ClientContext &clientContext)
		: _valid(clientContext._valid), _request(clientContext._request), _response(clientContext._response)
	{ }

	ClientContext(Request &request, Response &response)
		: _valid(true), _request(request), _response(response)
	{ }

	virtual ~ClientContext()
	{ }

	virtual bool isValid()
	{
		return this->_valid;
	}

	Request &request()
	{
		return this->_request;
	}

	Response &response()
	{
		return this->_response;
	}

	void process()
	{
		LOG_INFO("PERFORMED!");
		LOG_INFO("<html>Test</html>");
		this->response() << "<html>Test</html>";
		//icarus::routes::find(*this);
	}
};
}
}

#endif //ICARUSFRAMEWORK_CLIENT_HPP
