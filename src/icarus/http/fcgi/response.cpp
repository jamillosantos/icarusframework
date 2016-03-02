/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 21, 2016
 **/

#include <icarus/http/fcgi/response.h>

icarus::http::fcgi::response::response()
	: icarus::http::response::response(), _streambuf(nullptr)
{ }

icarus::http::fcgi::response::response(response &response)
	: _streambuf(response._streambuf)
{
	this->ostream = response.ostream;
}

icarus::http::fcgi::response::~response()
{
	LOG_TRACE("~Response");
	this->flush();
	LOG_TRACE("flushed");

	if (this->ostream)
		delete this->ostream;

	if (this->_streambuf)
		delete this->_streambuf;
}

void icarus::http::fcgi::response::init(FCGX_Request &fcgiRequest)
{
	this->_streambuf = new fcgi_streambuf(fcgiRequest.out);
	this->ostream = new std::ostream(this->_streambuf);
}
