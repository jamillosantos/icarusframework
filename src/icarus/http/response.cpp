/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#include <icarus/http/response.h>

void icarus::http::response::flush_headers()
{
	(*this->ostream) << "HTTP/1.1 " << this->status.code << " " << this->status.value << endh;
	for (values_value &header : this->headers())
	{
		(*this->ostream) << header.name() << ": " << header.value() << endh;
	}
	(*this->ostream) << endh;

	this->_header_sent = true;
}

void icarus::http::response::flush_headers(icarus::result &result)
{
	(*this->ostream) << "HTTP/1.1 " << this->status.code << " " << this->status.value << endh;
	for (values_value &header : result.headers())
	{
		(*this->ostream) << header.name() << ": " << header.value() << endh;
	}
	(*this->ostream) << endh;

	this->_header_sent = true;
}

icarus::http::response::response()
	: status(statuses::OK), _header_sent(false), ostream(nullptr)
{ }

icarus::http::response::~response()
{ }

icarus::http::value_list<icarus::http::values_value> &icarus::http::response::headers()
{
	return this->_headers;
}

void icarus::http::response::flush()
{
	if (!this->_header_sent)
		this->flush_headers();
	this->ostream->flush();
}

std::string icarus::http::response::endh("\r\n");
