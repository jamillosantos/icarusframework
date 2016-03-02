/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 21, 2016
 **/

#include <icarus/http/fcgi/application.h>

void icarus::http::fcgi::application::init()
{
	FCGX_Init();
}

icarus::http::client_context *icarus::http::fcgi::application::accept()
{
	LOG_TRACE("Accept(): " << std::this_thread::get_id());
	FCGX_Request *fcgiRequest = new FCGX_Request();
	FCGX_InitRequest(fcgiRequest, 0, 0);
	int acceptStatus = FCGX_Accept_r(fcgiRequest);
	if (acceptStatus == 0)
	{
		LOG_TRACE("Accepted: " << std::this_thread::get_id());
		http::fcgi::client_context *result = new icarus::http::fcgi::client_context(fcgiRequest);
		LOG_TRACE("Initializing: " << std::this_thread::get_id());
		result->init();
		return result;
	}
	LOG_TRACE("Rejected with: " << acceptStatus);
	FCGX_Free(fcgiRequest, true);
	return nullptr;
}
