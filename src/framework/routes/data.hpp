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

class ComposedURI
{
private:
	std::vector<RegexToken> _tokens;
public:
	ComposedURI &add(std::string name, std::string regex)
	{
		this->_tokens.emplace_back(name, regex);
		return *this;
	}

	ComposedURI &add(std::string expression)
	{
		this->_tokens.emplace_back(expression);
		return *this;
	}

	std::vector<RegexToken> &tokens()
	{
		return this->_tokens;
	}

	bool match(std::string requestUri)
	{
		// TODO
		return false;
	}

	std::string str()
	{
		std::stringstream stream;
		for (RegexToken &t : this->_tokens)
		{
			if (t.name().empty())
				stream << t.regex();
			else
				stream << "<" << t.name() << ":" << t.regex() << ">";
		}
		return stream.str();
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
	ComposedURI _composedURI;
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

	ComposedURI &uri()
	{
		return this->_composedURI;
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
public:
	Routes(size_t line)
		: Piece(line)
	{ }

	const std::vector<std::unique_ptr<Piece>> &pieces() const
	{
		return this->_pieces;
	}

	Piece *add(Piece *piece)
	{
		this->_pieces.emplace_back(piece);
	}
};

class Group
	: public Routes
{
private:
	ComposedURI _composedURI;
public:
	Group(size_t line)
		: Routes(line)
	{ }

	ComposedURI &uri()
	{
		return this->_composedURI;
	}
};

class Document
	: public Routes
{
private:
	std::string _name;
public:
	Document(const std::string &name)
		: Routes(0), _name(name)
	{ }

	const std::string &name()
	{
		return this->_name;
	}

	Document &name(std::string name)
	{
		this->_name = name;
		return *this;
	}
};

}
}

#endif // ICARUSFRAMEWORK_ROUTES_DATA_HPP
