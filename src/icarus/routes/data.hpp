/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#ifndef ICARUSFRAMEWORK_ROUTES_DATA_HPP
#define ICARUSFRAMEWORK_ROUTES_DATA_HPP

#define INPUT_STREAM_BUFFER_SIZE 4096

#include <string>
#include <vector>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include "../http/headers.hpp"
#include "../log.hpp"

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
	int _index;
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

	RegexToken &regex(const std::string &regex)
	{
		this->_regex = regex;
		return *this;
	}

	const int index() const
	{
		return this->_index;
	}

	RegexToken &index(int index)
	{
		this->_index = index;
		return *this;
	}
};

class ComposedURI
{
private:
	std::vector<RegexToken> _tokens;

	std::string _prefix;
	std::string _sufix;

	std::unique_ptr<boost::regex> regex;
protected:
	virtual void compile()
	{
		std::string pattern(this->_prefix);
		int
			i = 1,
			j = 0;
		for (RegexToken &token : this->_tokens)
		{
			if (token.name().empty())
				pattern += token.regex();
			else
			{
				pattern += "(";
				if (token.regex().empty())
				{
					if (j+1 < this->_tokens.size())
					{
						const RegexToken &nextToken = this->_tokens[j+1];
						if (nextToken.name().empty())
						{
							std::string nregex("[^");
							nregex += nextToken.regex()[0];
							nregex += "]+";
							pattern += nregex;
							token.regex(nregex);
						}
						else
						{
							// TODO Thrown an exception.
							LOG_ERROR("cannot have two parameter without separation.");
							std::exit(0);
						}
					}
					else
					{
						pattern += ".+";
						token.regex(".+");
					}
				}
				else
					pattern += token.regex();
				pattern += ")";
				token.index(i);
				++i;
			}
			j++;
		}
		pattern += this->_sufix;
		LOG_TRACE("Preparing regex: " << pattern);
		this->regex.reset(new boost::regex(pattern));
	}

public:
	ComposedURI()
	{ }

	ComposedURI(const ComposedURI &uri)
		: _tokens(uri._tokens), _prefix(uri._prefix), _sufix(uri._sufix)
	{
		if (uri.regex)
			this->regex.reset(new boost::regex(*uri.regex));
	}

	ComposedURI(const std::string &prefix, const std::string &sufix)
		: _prefix(prefix), _sufix(sufix)
	{ }

	ComposedURI &add(std::string name, std::string regex)
	{
		this->_tokens.emplace_back(name, regex);
		this->regex.reset();
		return *this;
	}

	ComposedURI &add(std::string expression)
	{
		this->_tokens.emplace_back(expression);
		this->regex.reset();
		return *this;
	}

	std::vector<RegexToken> &tokens()
	{
		return this->_tokens;
	}

	const std::string prefix() const
	{
		return this->_prefix;
	}

	ComposedURI &prefix(const std::string &prefix)
	{
		this->_prefix = prefix;
		return *this;
	}

	const std::string sufix() const
	{
		return this->_sufix;
	}

	ComposedURI &sufix(const std::string &sufix)
	{
		this->_sufix = sufix;
		return *this;
	}

	bool match(std::string requestUri, icarus::http::Values<http::Value> &params)
	{
		if (!this->regex)
			this->compile();

		if (this->regex)
		{
			boost::smatch results;

			if (boost::regex_match(requestUri, results, *this->regex))
			{
				for (RegexToken &token : this->_tokens)
				{
					if (!token.name().empty())
					{
						LOG_TRACE("Adding " << token.name() << ":" << results[token.index()])
						params.set(token.name(), results[token.index()]);
					}
				}
				return true;
			}
		}
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

	ComposedURI &operator=(const ComposedURI &uri)
	{
		this->_prefix = uri._prefix;
		this->_sufix = uri._sufix;
		for (const RegexToken &token : uri._tokens)
		{
			this->add(token.name(), token.regex());
		}
		if (uri.regex)
			this->regex.reset(new boost::regex(*uri.regex));
		else
			this->regex.reset();
		return *this;
	}
};

/**
 * Class that represents the parameters of a method called in a route.
 */
enum class MethodParamType
{
	NORMAL, REFERENCE, POINTER
};

class MethodParam
{
private:
	std::string _type;
	MethodParamType _attribute;
	std::string _name;

public:

	MethodParam()
	{ }

	MethodParam(const std::string type, const MethodParamType attribute, const std::string name)
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

	const MethodParamType &attribute() const
	{
		return this->_attribute;
	}

	MethodParam &attribute(const MethodParamType &attr)
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

	MethodParam &operator=(const MethodParam &param)
	{
		this->_attribute = param._attribute;
		this->_name = param._name;
		this->_type = param._type;
		return *this;
	}
};

/**
 * Class that represents the calling method in a route.
 */
class CallMethod
{
private:
	bool _static;

	std::vector<std::string> _path;
	std::string _name;

	std::vector<MethodParam> _params;
public:
	CallMethod()
	{ }

	CallMethod(const CallMethod &callMethod)
		: _path(callMethod._path), _name(callMethod._name), _params(callMethod._params)
	{ }

	bool isStatic()
	{
		return this->_static;
	}

	CallMethod &setStatic(bool _static)
	{
		this->_static = _static;
		return *this;
	}

	const std::vector<std::string> &path() const
	{
		return this->_path;
	}

	CallMethod &path(const std::string &path)
	{
		unsigned long
			lFind = 0,
			cFind = path.find("::");
		while (cFind != std::string::npos)
		{
			this->_path.emplace_back(path.substr(lFind, cFind - lFind));
			lFind = cFind + 2;
			cFind = path.find("::", lFind);
		}
		this->_name = path.substr(lFind, path.size() - lFind);
		return *this;
	}

	const std::string &name() const
	{
		return this->_name;
	}

	CallMethod &name(const std::string &name)
	{
		this->_name = name;
		return *this;
	}

	const std::vector<MethodParam> &params() const
	{
		return this->_params;
	}

	CallMethod &add(std::string type, MethodParamType attribute, std::string name)
	{
		this->_params.emplace_back(type, attribute, name);
		return *this;
	}

	CallMethod &operator=(const CallMethod &method)
	{
		this->_name = method._name;
		this->_params = method._params;			// TODO
		this->_path = method._path;				// TODO
		this->_static = method._static;
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

	virtual Piece *match(std::string method, std::string requestUri, http::Values<http::Value> &values) = 0;
};

class Routes;

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
	Route(Route &route)
		: Piece(route.line()), _httpMethod(route._httpMethod), _composedURI(route._composedURI),
		  _callMethod(route._callMethod)
	{ }

	Route(size_t line)
		: Piece(line), _composedURI("^", "$")
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

	Route &callMethod(CallMethod &callMethod)
	{
		this->_callMethod = callMethod;
		return *this;
	}

	virtual Piece *match(std::string method, std::string uri, http::Values<http::Value> &params) override
	{
		LOG_TRACE("Route::match: " << this->uri().str());
		if (this->httpMethod() == method)
		{
			if (this->uri().match(uri, params))
				return this;
		}
		return nullptr;
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

	virtual Piece *add(Piece *piece)
	{
		this->_pieces.emplace_back(piece);
		return piece;
	}

	virtual Piece *match(std::string method, std::string requestUri, http::Values<http::Value> &values)
	{
		LOG_TRACE("routes::match");
		Piece *result = nullptr;
		for (const std::unique_ptr<Piece> &piece : this->_pieces)
		{
			result = piece->match(method, requestUri, values);
			if (result)
				break;
		}
		return result;
	}
};

class Group
	: public Routes
{
private:
	ComposedURI _composedURI;
public:
	Group(size_t line)
		: Routes(line), _composedURI("^", "")
	{ }

	ComposedURI &uri()
	{
		return this->_composedURI;
	}
/*
	virtual Piece* match(std::string method, std::string uri, http::Values<http::Value> &params) override
	{
		LOG_TRACE("Group::match - " << this->_composedURI.str());
		if (this->_composedURI.match(uri, params))
			return Routes::match(method, uri, params);
		return nullptr;
	}
*/
};

class Document
	: public Routes
{
private:
	std::string _name;

	std::vector<std::string> _packages;
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

	std::vector<std::string> &packages()
	{
		return this->_packages;
	}
};
}
}

#endif // ICARUSFRAMEWORK_ROUTES_DATA_HPP
