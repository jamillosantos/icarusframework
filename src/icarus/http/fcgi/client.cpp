/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 22, 2016
 **/

#include <icarus/http/fcgi/client.h>

icarus::http::fcgi::client_context::client_context(FCGX_Request *fcgiRequest)
	: icarus::http::client_context::client_context(*(this->_fcgiRequest = new fcgi::request()),
	  *(this->_fcgiResponse = new fcgi::response())), _fcgxRequest(fcgiRequest)
{ }

icarus::http::fcgi::client_context::~client_context()
{
	delete this->_fcgiRequest;
	delete this->_fcgiResponse;
	FCGX_Finish_r(_fcgxRequest);
}

void icarus::http::fcgi::client_context::init()
{
	this->_fcgiRequest->init(*this->_fcgxRequest);
	this->_fcgiResponse->init(*this->_fcgxRequest);
}
