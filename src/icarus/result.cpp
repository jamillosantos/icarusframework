/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#include <icarus/result.h>

#include <json/writer.h>

icarus::result::result()
	: icarus::content::content(), _status(icarus::statuses::OK)
{ }

icarus::result::result(icarus::status &status)
	: icarus::content::content(), _status(status)
{ }

icarus::result::result(icarus::status &status, const std::string &content)
	: icarus::content::content(content), _status(status)
{ }

icarus::result::result(const std::string &content)
	: icarus::result::result(icarus::statuses::OK, content)
{ }

icarus::result::result(icarus::status &status, const Json::Value &json)
	: icarus::result::result(status)
{
	this->_contentType = "application/json";
	this->stream() << json;
}

icarus::result::result(const Json::Value &json)
	: icarus::result::result(icarus::statuses::OK, json)
{ }

icarus::result::result(icarus::result &&r)
	: icarus::content::content(r), _status(r._status)
{
	LOG_DEBUG("icarus::result::result(icarus::result &&r)");
}

const icarus::http::headers &icarus::result::headers() const
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

icarus::result &icarus::result::header(const std::string &name, const std::string &value)
{
	this->_headers.emplace(name, value);
	return *this;
}

icarus::result icarus::results::status(icarus::status &status, const std::string &content)
{
	return icarus::result(status, content);
}

icarus::result &icarus::results::status(icarus::result &result, icarus::status &status)
{
	return result.status(status);
}

icarus::result icarus::results::ok(const std::string &content)
{
	return icarus::result(icarus::statuses::OK, content);
}

icarus::result &&icarus::results::ok(icarus::result &result)
{
	return std::move(result.status(icarus::statuses::OK));
}

icarus::result icarus::results::ok(const Json::Value &json)
{
	return icarus::result(icarus::statuses::OK, json);
}

icarus::result icarus::results::notImplemented(const std::string &content)
{
	return icarus::result(icarus::statuses::NOT_IMPLEMENTED, content);
}

icarus::result &icarus::results::notImplemented(icarus::result &result)
{
	return icarus::results::status(result, icarus::statuses::NOT_IMPLEMENTED);
}

icarus::result icarus::results::notImplemented(const Json::Value &json)
{
	return icarus::result(icarus::statuses::NOT_IMPLEMENTED, json);
}

icarus::result icarus::results::badRequest(const std::string &content)
{
	return icarus::result(icarus::statuses::BAD_REQUEST, content);
}

icarus::result &icarus::results::badRequest(icarus::result &result)
{
	return icarus::results::status(result, icarus::statuses::BAD_REQUEST);
}

icarus::result icarus::results::badRequest(const Json::Value &json)
{
	return icarus::result(icarus::statuses::BAD_REQUEST, json);
}

icarus::result icarus::results::unauthorized(const std::string &content)
{
	return icarus::result(icarus::statuses::UNAUTHORIZED, content);
}

icarus::result &icarus::results::unauthorized(icarus::result &result)
{
	return icarus::results::status(result, icarus::statuses::UNAUTHORIZED);
}

icarus::result icarus::results::unauthorized(const Json::Value &json)
{
	return icarus::result(icarus::statuses::UNAUTHORIZED, json);
}

icarus::result icarus::results::paymentRequired(const std::string &content)
{
	return icarus::result(icarus::statuses::PAYMENT_REQUIRED, content);
}

icarus::result &icarus::results::paymentRequired(icarus::result &result)
{
	return icarus::results::status(result, icarus::statuses::PAYMENT_REQUIRED);
}

icarus::result icarus::results::paymentRequired(const Json::Value &json)
{
	return icarus::result(icarus::statuses::PAYMENT_REQUIRED, json);
}

icarus::result icarus::results::forbidden(const std::string &content)
{
	return icarus::result(icarus::statuses::FORBIDDEN, content);
}

icarus::result &icarus::results::forbidden(icarus::result &result)
{
	return icarus::results::status(result, icarus::statuses::FORBIDDEN);
}

icarus::result icarus::results::forbidden(const Json::Value &json)
{
	return icarus::result(icarus::statuses::FORBIDDEN, json);
}

icarus::result icarus::results::notFound(const std::string &content)
{
	return icarus::result(icarus::statuses::NOT_FOUND, content);
}

icarus::result &icarus::results::notFound(icarus::result &result)
{
	return icarus::results::status(result, icarus::statuses::NOT_FOUND);
}

icarus::result icarus::results::notFound(const Json::Value &json)
{
	return icarus::result(icarus::statuses::NOT_FOUND, json);
}

icarus::result icarus::results::internalServerError(const std::string &content)
{
	return icarus::result(icarus::statuses::INTERNAL_ERROR, content);
}

icarus::result &icarus::results::internalServerError(icarus::result &result)
{
	return icarus::results::status(result, icarus::statuses::INTERNAL_ERROR);
}

icarus::result icarus::results::internalServerError(const Json::Value &json)
{
	return icarus::result(icarus::statuses::INTERNAL_ERROR, json);
}

icarus::result icarus::results::movedPermanently(std::string url)
{
	icarus::result result(icarus::statuses::MOVE_PERMANENTLY);
	result.header("Location", url);
	return result;
}

icarus::result icarus::results::redirect(std::string url)
{
	icarus::result result(icarus::statuses::TEMPORARY_REDIRECT);
	result.header("Location", url);
	return result;
}

icarus::result icarus::results::temporaryRedirect(std::string url)
{
	icarus::result result(icarus::statuses::TEMPORARY_REDIRECT);
	result.header("Location", url);
	return result;
}

std::ostream &operator<<(std::ostream &out, icarus::result &result)
{
	out << result.stream().rdbuf();
	return out;
}

