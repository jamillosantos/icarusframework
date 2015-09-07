/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#ifndef ICARUSFRAMEWORK_RESULT_HPP
#define ICARUSFRAMEWORK_RESULT_HPP

#include <boost/optional/optional.hpp>
#include "../../minotaur/src/ochainstream.hpp"
#include "http/statuses.h"

namespace icarus
{
class Result;

namespace results
{
public:
	static Result status(http::Status status, minotaur::OChainStream content)
	{
		return Result(status, content);
	}

	static Result status(http::Status status, std::string content)
	{
		return Result(status, content);
	}

	static Result ok(minotaur::OChainStream content)
	{
		return Result(http::OK, content);
	}

	static Result ok(std::string content)
	{
		return Result(http::OK, content);
	}

	static Result notImplemented(minotaur::OChainStream content)
	{
		return Result(http::NOT_IMPLEMENTED, content);
	}

	static Result notImplemented(std::string content)
	{
		return Result(http::NOT_IMPLEMENTED, content);
	}

	static Result badRequest(minotaur::OChainStream content)
	{
		return Result(http::BAD_REQUEST, content);
	}

	static Result badRequest(std::string content)
	{
		return Result(http::BAD_REQUEST, content);
	}

	static Result unauthorized(minotaur::OChainStream content)
	{
		return Result(http::UNAUTHORIZED, content);
	}

	static Result unauthorized(std::string content)
	{
		return Result(http::UNAUTHORIZED, content);
	}

	static Result paymentRequired(minotaur::OChainStream content)
	{
		return Result(http::PAYMENT_REQUIRED, content);
	}

	static Result paymentRequired(std::string content)
	{
		return Result(http::PAYMENT_REQUIRED, content);
	}

	static Result forbidden(minotaur::OChainStream content)
	{
		return Result(http::FORBIDDEN, content);
	}

	static Result forbidden(std::string content)
	{
		return Result(http::FORBIDDEN, content);
	}

	static Result notFound(minotaur::OChainStream content)
	{
		return Result(http::NOT_FOUND, content);
	}

	static Result notFound(std::string content)
	{
		return Result(http::NOT_FOUND, content);
	}

	static Result internalServerError(minotaur::OChainStream content)
	{
		return Result(http::INTERNAL_ERROR, content);
	}

	static Result internalServerError(std::string content)
	{
		return Result(http::INTERNAL_ERROR, content);
	}

	static Result movedPermanently(std::string url)
	{
		std::cerr << "TODO" << std::endl;
		throw std::exception();
	}

	static Result redirect(std::string url)
	{
		std::cerr << "TODO" << std::endl;
		throw std::exception();
	}

	static Result temporaryRedirect(std::string url)
	{
		std::cerr << "TODO" << std::endl;
		throw std::exception();
	}
};

class Result
{
private:
	http::Status _status;
	minotaur::OChainStream output;
	boost::optional<std::string> _contentType;
	boost::optional<std::string> _charset;

	std::vector<std::pair<std::string, std::string>> headers;
public:
	Result(http::Status status, minotaur::OChainStream &output)
		: _status(status), output(output)
	{ }

	Result(minotaur::OChainStream &output)
		: Result(http::OK, output)
	{ }

	Result(http::Status status, std::string result)
		: _status(status)
	{
		this->output << result;
	}

	Result(std::string result)
		: Result(http::OK, result)
	{ }

	Result& header(std::string name, std::string value)
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

	Result& status(http::Status status)
	{
		this->_status = status;
	}

	http::Status status() const
	{
		return this->_status;
	}

	Result& charset(std::string &charset)
	{
		this->_charset = charset;
	}

	boost::optional<std::string> charset() const
	{
		return this->_charset;
	}

	Result& as(std::string &contentType)
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
