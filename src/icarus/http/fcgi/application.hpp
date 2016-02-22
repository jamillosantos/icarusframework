/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 21, 2016
 **/

#include <icarus/application.hpp>
#include <fcgiapp.h>
#include "client.hpp"
#include "response.hpp"
#include "request.hpp"

#ifndef ICARUSFRAMEWORK_HTTP_FCGI_APPLICATION_HPP
#define ICARUSFRAMEWORK_HTTP_FCGI_APPLICATION_HPP

namespace icarus
{
namespace http
{
namespace fcgi
{
class Application
	: public icarus::Application
{
private:
	volatile int _acceptStatus;
	FCGX_Request _fcgiRequest;
protected:
	virtual void init() override
	{
		FCGX_Init();
		FCGX_InitRequest(&_fcgiRequest, 0, 0);
	}

	virtual http::ClientContext *accept() override
	{
		LOG_TRACE("Accept()");
		this->_acceptStatus = FCGX_Accept_r(&this->_fcgiRequest);
		if (this->_acceptStatus == 0)
		{
			LOG_TRACE("Accepted");
			http::fcgi::ClientContext *result = new icarus::http::fcgi::ClientContext();
			LOG_TRACE("Initializing");
			result->init(this->_fcgiRequest);
			return result;
		}
		LOG_TRACE("Rejected with: " << this->_acceptStatus);
		return nullptr;
	}
};

}
}
}

#endif //ICARUSFRAMEWORK_HTTP_FCGI_APPLICATION_HPP
