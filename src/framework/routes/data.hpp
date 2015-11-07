/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#ifndef ICARUSFRAMEWORK_ROUTES_DATA_HPP
#define ICARUSFRAMEWORK_ROUTES_DATA_HPP

#define INPUT_STREAM_BUFFER_SIZE 4096

#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include <stdexcept>
#include <fstream>

namespace icarus
{
namespace routes
{

/**
 * Regex tokens in a route rule.
 */
class RegexToken
{
private:
	std::string _name;
	std::string _regex;
public:
	RegexToken(const std::string &name, const std::string &regex)
		: _name(name), _regex(regex)
	{ }

	RegexToken(const std::string &regex)
		: _name(""), _regex(regex)
	{ }

	const std::string &name() const
	{
		return this->_name;
	}

	const std::string &regex() const
	{
		return this->_regex;
	}
};

/**
 * Class that represents the parameters of a method called in a route.
 */
class MethodParam
{
public:
	enum MethodType { NORMAL, REFERENCE, POINTER };

	MethodParam()
	{ }

	MethodParam(const std::string type, const MethodType attribute, const std::string name)
		: _type(type), _attribute(attribute), _name(name)
	{ }

	const std::string &type() const
	{
		return this->_type;
	}

	MethodParam &type(const std::string &type)
	{
		this->_type = type;
		return *this;
	}

	const MethodType &attribute() const
	{
		return this->_attribute;
	}

	MethodParam &attribute(const MethodType &attr)
	{
		this->_attribute = attr;
		return *this;
	}

	const std::string &name() const
	{
		return this->_name;
	}

	MethodParam &name(const std::string &name)
	{
		this->_name = name;
		return *this;
	}

private:
	std::string _type;
	MethodType _attribute;
	std::string _name;
};

/**
 * Class that represents the calling method in a route.
 */
class CallMethod
{
private:
	bool _static;

	std::string _path;

	std::vector<MethodParam> _params;
public:
	CallMethod()
	{ }

	CallMethod(CallMethod &callMethod)
		: _path(callMethod._path), _params(callMethod._params)
	{ }

	CallMethod(const CallMethod &callMethod)
		: _path(callMethod._path), _params(callMethod._params)
	{ }

	bool isStatic()
	{
		return this->_static;
	}

	CallMethod& setStatic(bool _static)
	{
		this->_static = _static;
		return *this;
	}

	const std::string &path() const
	{
		return this->_path;
	}

	CallMethod& path(const std::string &path)
	{
		this->_path = path;
		return *this;
	}

	const std::vector<MethodParam> &params() const
	{
		return this->_params;
	}

	CallMethod &add(std::string type, MethodParam::MethodType attribute, std::string name)
	{
		this->_params.emplace_back(type, attribute, name);
		return *this;
	}
};

/**
 * Abstraction of each part of a routes.
 */
class Piece
{
private:
	size_t _line;
public:
	Piece(size_t line)
		: _line(line)
	{ }

	virtual ~Piece()
	{ }

	size_t line() const
	{
		return this->_line;
	}

	void line(size_t line)
	{
		this->_line = line;
	}
};

/**
 * Represents a route.
 */
class Route
	: public Piece
{
private:
	std::string _httpMethod;
	std::vector<RegexToken> _regex;
	CallMethod _callMethod;
public:
	Route(size_t line)
		: Piece(line)
	{ }

	const std::string &httpMethod() const
	{
		return this->_httpMethod;
	}

	Route &httpMethod(const std::string &httpMethod)
	{
		this->_httpMethod = httpMethod;
		return *this;
	}

	const std::vector<RegexToken> &regex() const
	{
		return this->_regex;
	}

	Route & add(std::string name, std::string regex)
	{
		this->_regex.emplace_back(name, regex);
		return *this;
	}

	Route & add(std::string expression)
	{
		this->_regex.emplace_back(expression);
		return *this;
	}

	const CallMethod &callMethod() const
	{
		return this->_callMethod;
	}

	Route & callMethod(CallMethod &callMethod)
	{
		this->_callMethod = callMethod;
		return *this;
	}
};

class Routes
	: public Piece
{
private:
	std::vector<std::unique_ptr<Piece>> _pieces;

	std::vector<std::string> _package;

	std::string _prefix;
public:
	Routes(size_t line)
		: Piece(line)
	{ }

	Routes()
		: Routes(0)
	{ }

	const std::vector<std::unique_ptr<Piece>> &pieces() const
	{
		return this->_pieces;
	}

	Piece *add(Piece *piece)
	{
		this->_pieces.emplace_back(piece);
	}

	const std::vector<std::string> &package() const
	{
		return this->_package;
	}

	const std::string &prefix() const
	{
		return this->_prefix;
	}

	Routes &prefix(std::string prefix)
	{
		this->_prefix = prefix;
	}
};


class Group
	: public Routes
{
private:
	std::string _uri;
public:
	Group(std::string uri, size_t line)
		: Routes(line), _uri(uri)
	{ }

	const std::string uri() const
	{
		return this->_uri;
	}
};

}
}

#endif // ICARUSFRAMEWORK_ROUTES_DATA_HPP
