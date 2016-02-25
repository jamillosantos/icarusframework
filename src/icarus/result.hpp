/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#ifndef ICARUSFRAMEWORK_RESULT_HPP
#define ICARUSFRAMEWORK_RESULT_HPP

#include <boost/optional/optional.hpp>
#include "http/statuses.hpp"
#include "../minotaur/ochainstream.hpp"

namespace icarus
{
class result;

namespace results
{
public:
	static result status(http::status status, minotaur::OChainStream content)
	{
		return result(status, content);
	}

	static result status(http::status status, std::string content)
	{
		return result(status, content);
	}

	static result ok(minotaur::OChainStream content)
	{
		return result(http::statuses::OK, content);
	}

	static result ok(std::string content)
	{
		return result(http::statuses::OK, content);
	}

	static result notImplemented(minotaur::OChainStream content)
	{
		return result(http::statuses::NOT_IMPLEMENTED, content);
	}

	static result notImplemented(std::string content)
	{
		return result(http::statuses::NOT_IMPLEMENTED, content);
	}

	static result badRequest(minotaur::OChainStream content)
	{
		return result(http::statuses::BAD_REQUEST, content);
	}

	static result badRequest(std::string content)
	{
		return result(http::statuses::BAD_REQUEST, content);
	}

	static result unauthorized(minotaur::OChainStream content)
	{
		return result(http::statuses::UNAUTHORIZED, content);
	}

	static result unauthorized(std::string content)
	{
		return result(http::statuses::UNAUTHORIZED, content);
	}

	static result paymentRequired(minotaur::OChainStream content)
	{
		return result(http::statuses::PAYMENT_REQUIRED, content);
	}

	static result paymentRequired(std::string content)
	{
		return result(http::statuses::PAYMENT_REQUIRED, content);
	}

	static result forbidden(minotaur::OChainStream content)
	{
		return result(http::statuses::FORBIDDEN, content);
	}

	static result forbidden(std::string content)
	{
		return result(http::statuses::FORBIDDEN, content);
	}

	static result notFound(minotaur::OChainStream content)
	{
		return result(http::statuses::NOT_FOUND, content);
	}

	static result notFound(std::string content)
	{
		return result(http::statuses::NOT_FOUND, content);
	}

	static result internalServerError(minotaur::OChainStream content)
	{
		return result(http::statuses::INTERNAL_ERROR, content);
	}

	static result internalServerError(std::string content)
	{
		return result(http::statuses::INTERNAL_ERROR, content);
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
};

class result
{
private:
	http::status &_status;
	minotaur::OChainStream output;
	boost::optional<std::string> _contentType;
	boost::optional<std::string> _charset;

	std::vector<std::pair<std::string, std::string>> headers;
public:
	result(const http::status &status, minotaur::OChainStream &output)
		: _status(status), output(output)
	{ }

	result(minotaur::OChainStream &output)
		: result(http::statuses::OK, output)
	{ }

	result(const http::status &status, std::string result)
		: _status(status)
	{
		this->output << result;
	}

	result(std::string result)
		: result(http::statuses::OK, result)
	{ }

	result& header(std::string name, std::string value)
	{
		this->headers.emplace_back(std::make_pair(name, value));
		return *this;
	}

	boost::optional<std::string> header(std::string name) const
	{
		for (auto &h : this->headers)
		{
			if (h.first == name)
				return h.second;
		}
		return nullptr;
	}

	result& status(const http::status &status)
	{
		this->_status = status;
	}

	http::status status() const
	{
		return this->_status;
	}

	result& charset(std::string &charset)
	{
		this->_charset = charset;
	}

	boost::optional<std::string> charset() const
	{
		return this->_charset;
	}

	result& as(std::string &contentType)
	{
		this->_contentType = contentType;
	}

	boost::optional<std::string> contentType() const
	{
		return this->_contentType;
	}
};

}

#endif // ICARUSFRAMEWORK_RESULT_HPP
