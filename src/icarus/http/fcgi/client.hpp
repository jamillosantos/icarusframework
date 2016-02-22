/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 22, 2016
 **/

#ifndef ICARUSFRAMEWORK_HTTP_FCGI_CLIENT_HPP
#define ICARUSFRAMEWORK_HTTP_FCGI_CLIENT_HPP

#include <icarus/http/client.hpp>
#include <fcgiapp.h>
#include "request.hpp"
#include "response.hpp"

namespace icarus
{
namespace http
{
namespace fcgi
{
class ClientContext
	: public icarus::http::ClientContext
{
private:
	fcgi::Request _fcgiRequest;
	fcgi::Response _fcgiResponse;
public:
	ClientContext()
		: icarus::http::ClientContext::ClientContext(_fcgiRequest, _fcgiResponse)
	{ }

	virtual ~ClientContext() override
	{
		LOG_TRACE("~ClientContext");
	}

	void init(FCGX_Request &request)
	{
		this->_fcgiRequest.init(request);
		LOG_TRACE("Request URI: " << this->_fcgiRequest.uri());
		this->_fcgiResponse.init(request);
	}
};
}
}
}

#endif //ICARUSFRAMEWORK_HTTP_FCGI_CLIENT_HPP
