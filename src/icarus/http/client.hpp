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
class client_context
{
protected:
	bool _valid;

	request &_request;
	response &_response;
public:
	client_context(client_context &clientContext)
		: _valid(clientContext._valid), _request(clientContext._request), _response(clientContext._response)
	{ }

	client_context(request &req, response &resp)
		: _valid(true), _request(req), _response(resp)
	{ }

	virtual ~client_context()
	{ }

	virtual bool isValid()
	{
		return this->_valid;
	}

	request &req()
	{
		return this->_request;
	}

	response &resp()
	{
		return this->_response;
	}

	void process()
	{
		LOG_INFO("PERFORMED!");
		LOG_INFO("<html>Test</html>");
		this->resp() << "<html>Test</html>";
		//icarus::routes::find(*this);
	}
};
}
}

#endif //ICARUSFRAMEWORK_CLIENT_HPP
