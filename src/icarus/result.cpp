/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#include <icarus/result.h>

icarus::result::result(icarus::status &status)
	: icarus::content::content(), _status(status)
{ }

icarus::result::result(icarus::status &status, const std::string &content)
	: icarus::content::content(content), _status(status)
{ }

icarus::result::result(const std::string &content)
	: icarus::result::result(icarus::statuses::OK, content)
{ }

icarus::result::result(icarus::result &&r)
	: icarus::content::content(r), _status(r._status)
{ }

icarus::http::value_list<icarus::http::values_value> &icarus::result::headers()
{
	return this->_headers;
}

icarus::result &icarus::result::status(icarus::status &status)
{
	this->_status = status;
	return *this;
}

icarus::status &icarus::result::status() const
{
	return this->_status;
}

icarus::result &icarus::result::charset(std::string &charset)
{
	this->_charset = charset;
}

boost::optional<std::string> icarus::result::charset() const
{
	return this->_charset;
}

icarus::result &icarus::result::as(std::string &contentType)
{
	this->_contentType = contentType;
}

boost::optional<std::string> icarus::result::contentType() const
{
	return this->_contentType;
}

icarus::result icarus::results::status(icarus::status &status, const std::string &content)
{
	return icarus::result(status, content);
}

icarus::result icarus::results::ok(const std::string &content)
{
	return icarus::result(icarus::statuses::OK, content);
}

icarus::result icarus::results::notImplemented(const std::string &content)
{
	return icarus::result(icarus::statuses::NOT_IMPLEMENTED, content);
}

icarus::result icarus::results::badRequest(const std::string &content)
{
	return icarus::result(icarus::statuses::BAD_REQUEST, content);
}

icarus::result icarus::results::unauthorized(const std::string &content)
{
	return icarus::result(icarus::statuses::UNAUTHORIZED, content);
}

icarus::result icarus::results::paymentRequired(const std::string &content)
{
	return icarus::result(icarus::statuses::PAYMENT_REQUIRED, content);
}

icarus::result icarus::results::forbidden(const std::string &content)
{
	return icarus::result(icarus::statuses::FORBIDDEN, content);
}

icarus::result icarus::results::notFound(const std::string &content)
{
	return icarus::result(icarus::statuses::NOT_FOUND, content);
}

icarus::result icarus::results::internalServerError(const std::string &content)
{
	return icarus::result(icarus::statuses::INTERNAL_ERROR, content);
}

icarus::result icarus::results::movedPermanently(std::string url)
{
	LOG_ERROR("TODO");
	throw std::exception();
}

icarus::result icarus::results::redirect(std::string url)
{
	LOG_ERROR("TODO");
	throw std::exception();
}

icarus::result icarus::results::temporaryRedirect(std::string url)
{
	LOG_ERROR("TODO");
	throw std::exception();
}

std::ostream &operator<<(std::ostream &out, icarus::result &result)
{
	out << result.stream().rdbuf();
	return out;
}

