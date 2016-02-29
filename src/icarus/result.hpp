/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#ifndef ICARUSFRAMEWORK_RESULT_HPP
#define ICARUSFRAMEWORK_RESULT_HPP

#include <boost/optional/optional.hpp>

#include <sstream>

#include <icarus/log.hpp>
#include <icarus/http/headers.hpp>
#include "statuses.hpp"

namespace icarus
{
class result
{
private:
	icarus::status &_status;
	std::stringstream _content_stream;
	boost::optional<std::string> _contentType;
	boost::optional<std::string> _charset;

	icarus::http::value_list<icarus::http::values_value> _headers;
public:

	result(icarus::status &status)
		: _status(status), _content_stream(std::stringstream::binary | std::stringstream::in | std::stringstream::out)
	{ }

	result(icarus::status &status, const std::string &content)
		: result(status)
	{
		this->_content_stream << content;
	}

	result(const std::string &content)
		: result(icarus::statuses::OK, content)
	{ }

	icarus::http::value_list<icarus::http::values_value> &headers()
	{
		return this->_headers;
	}

	result &status(icarus::status &status)
	{
		this->_status = status;
		return *this;
	}

	icarus::status &status() const
	{
		return this->_status;
	}

	result &charset(std::string &charset)
	{
		this->_charset = charset;
	}

	boost::optional<std::string> charset() const
	{
		return this->_charset;
	}

	result &as(std::string &contentType)
	{
		this->_contentType = contentType;
	}

	boost::optional<std::string> contentType() const
	{
		return this->_contentType;
	}

	std::stringstream &content()
	{
		return this->_content_stream;
	}
};

namespace results
{
static result status(icarus::status &status, const std::string &content)
{
	return result(status, content);
}

static result ok(const std::string &content)
{
	return result(statuses::OK, content);
}

static result notImplemented(const std::string &content)
{
	return result(statuses::NOT_IMPLEMENTED, content);
}

static result badRequest(const std::string &content)
{
	return result(statuses::BAD_REQUEST, content);
}

static result unauthorized(const std::string &content)
{
	return result(statuses::UNAUTHORIZED, content);
}

static result paymentRequired(const std::string &content)
{
	return result(statuses::PAYMENT_REQUIRED, content);
}

static result forbidden(const std::string &content)
{
	return result(statuses::FORBIDDEN, content);
}

static result notFound(const std::string &content)
{
	return result(statuses::NOT_FOUND, content);
}

static result internalServerError(const std::string &content)
{
	return result(statuses::INTERNAL_ERROR, content);
}

static result movedPermanently(std::string url)
{
	LOG_ERROR("TODO");
	throw std::exception();
}

static result redirect(std::string url)
{
	LOG_ERROR("TODO");
	throw std::exception();
}

static result temporaryRedirect(std::string url)
{
	LOG_ERROR("TODO");
	throw std::exception();
}
}
}

#endif // ICARUSFRAMEWORK_RESULT_HPP
