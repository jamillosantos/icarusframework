/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#include <icarus/http/response.h>

void icarus::http::response::flush_headers()
{
	(*this->ostream) << "HTTP/1.1 " << this->status.code << " " << this->status.value << endh;

	for (const icarus::http::headers::pair &header : this->headers())
	{
		for (const std::string &value : header.second)
			(*this->ostream) << header.first << ": " << value << endh;
	}
	for (const icarus::http::cookies::pair &cookie : this->_cookies)
	{
		if (cookie.second.expires())
		{
			std::time_t tt = std::chrono::system_clock::to_time_t(*cookie.second.expires());
			(*this->ostream) << "Set-Cookie: " << cookie.first << "=" << cookie.second.value() << "; Expires=" << std::put_time(std::localtime(&tt), "%a, %Od %b %Y %H:%I:%S %Z") << endh;
		}
		else
			(*this->ostream) << "Set-Cookie: " << cookie.first << "=" << cookie.second.value() << endh;
	}
	(*this->ostream) << endh;

	this->_header_sent = true;
}

void icarus::http::response::flush_headers(const icarus::result &result)
{
	(*this->ostream) << "HTTP/1.1 " << this->status.code << " " << this->status.value << endh;
	for (auto it = result.headers().cbegin(); it != result.headers().cend(); ++it)
	{
		for (const std::string &value : (*it).second)
			(*this->ostream) << (*it).first << ": " << value << endh;
	}
	for (const icarus::http::cookies::pair &cookie : this->_cookies)
	{
		if (cookie.second.expires())
		{
			std::time_t tt = std::chrono::system_clock::to_time_t(*cookie.second.expires());
			(*this->ostream) << "Set-Cookie: " << cookie.first << "=" << cookie.second.value() << "; Expires=" << std::put_time(std::localtime(&tt), "%a, %Od %b %Y %H:%I:%S %Z") << endh;
		}
		else
			(*this->ostream) << "Set-Cookie: " << cookie.first << "=" << cookie.second.value() << endh;
	}
	(*this->ostream) << endh;

	this->_header_sent = true;
}

icarus::http::response::response()
	: status(statuses::OK), _header_sent(false), ostream(nullptr)
{ }

icarus::http::response::~response()
{ }

icarus::http::headers &icarus::http::response::headers()
{
	return this->_headers;
}

icarus::http::cookies &icarus::http::response::cookies()
{
	return this->_cookies;
}

void icarus::http::response::flush()
{
	if (!this->_header_sent)
		this->flush_headers();
	this->ostream->flush();
}

std::string icarus::http::response::endh("\r\n");

icarus::http::response &icarus::http::response::operator<<(const icarus::result &result)
{
	if (!this->_header_sent)
		this->flush_headers(result);
	(*this->ostream) << result.stream().rdbuf();
	return *this;
}

icarus::http::response &icarus::http::response::operator<<(icarus::content &content)
{
	(*this->ostream) << content.stream().rdbuf();
	return *this;
}

icarus::http::response &icarus::http::response::operator<<(std::ostream &(*manip)(std::ostream &))
{
	if (!this->_header_sent)
		this->flush_headers();
	(*this->ostream) << manip;
	return *this;
}