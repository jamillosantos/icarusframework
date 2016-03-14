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

static icarus::result status(icarus::status &status, const std::string &content)
{
	return icarus::result(status, content);
}

static icarus::result ok(const std::string &content)
{
	return icarus::result(icarus::statuses::OK, content);
}

static icarus::result notImplemented(const std::string &content)
{
	return icarus::result(icarus::statuses::NOT_IMPLEMENTED, content);
}

static icarus::result badRequest(const std::string &content)
{
	return icarus::result(icarus::statuses::BAD_REQUEST, content);
}

static icarus::result unauthorized(const std::string &content)
{
	return icarus::result(icarus::statuses::UNAUTHORIZED, content);
}

static icarus::result paymentRequired(const std::string &content)
{
	return icarus::result(icarus::statuses::PAYMENT_REQUIRED, content);
}

static icarus::result forbidden(const std::string &content)
{
	return icarus::result(icarus::statuses::FORBIDDEN, content);
}

static icarus::result notFound(const std::string &content)
{
	return icarus::result(icarus::statuses::NOT_FOUND, content);
}

static icarus::result internalServerError(const std::string &content)
{
	return icarus::result(icarus::statuses::INTERNAL_ERROR, content);
}

static icarus::result movedPermanently(std::string url)
{
	LOG_ERROR("TODO");
	throw std::exception();
}

static icarus::result redirect(std::string url)
{
	LOG_ERROR("TODO");
	throw std::exception();
}

static icarus::result temporaryRedirect(std::string url)
{
	LOG_ERROR("TODO");
	throw std::exception();
}