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
protected:
	virtual void init() override
	{
		FCGX_Init();
	}

	virtual http::ClientContext *accept() override
	{
		LOG_TRACE("Accept(): " << std::this_thread::get_id());
		FCGX_Request *fcgiRequest = new FCGX_Request();
		FCGX_InitRequest(fcgiRequest, 0, 0);
		int acceptStatus = FCGX_Accept_r(fcgiRequest);
		if (acceptStatus == 0)
		{
			LOG_TRACE("Accepted: " << std::this_thread::get_id());
			http::fcgi::ClientContext *result = new icarus::http::fcgi::ClientContext(fcgiRequest);
			LOG_TRACE("Initializing: " << std::this_thread::get_id());
			result->init();
			return result;
		}
		LOG_TRACE("Rejected with: " << acceptStatus);
		FCGX_Free(fcgiRequest, true);
		return nullptr;
	}
};

}
}
}

#endif //ICARUSFRAMEWORK_HTTP_FCGI_APPLICATION_HPP
