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
class regex_token
{
private:
	std::string _name;
	std::string _regex;
	int _index;
public:
	regex_token(const std::string &name, const std::string &regex)
		: _name(name), _regex(regex)
	{ }

	regex_token(const std::string &regex)
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

	regex_token &regex(const std::string &regex)
	{
		this->_regex = regex;
		return *this;
	}

	const int index() const
	{
		return this->_index;
	}

	regex_token &index(int index)
	{
		this->_index = index;
		return *this;
	}
};

class composed_uri
{
private:
	std::vector<regex_token> _tokens;

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
		for (regex_token &token : this->_tokens)
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
						const regex_token &nextToken = this->_tokens[j+1];
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
	composed_uri()
	{ }

	composed_uri(const composed_uri &uri)
		: _tokens(uri._tokens), _prefix(uri._prefix), _sufix(uri._sufix)
	{
		if (uri.regex)
			this->regex.reset(new boost::regex(*uri.regex));
	}

	composed_uri(const std::string &prefix, const std::string &sufix)
		: _prefix(prefix), _sufix(sufix)
	{ }

	composed_uri &add(std::string name, std::string regex)
	{
		this->_tokens.emplace_back(name, regex);
		this->regex.reset();
		return *this;
	}

	composed_uri &add(std::string expression)
	{
		this->_tokens.emplace_back(expression);
		this->regex.reset();
		return *this;
	}

	std::vector<regex_token> &tokens()
	{
		return this->_tokens;
	}

	const std::string prefix() const
	{
		return this->_prefix;
	}

	composed_uri &prefix(const std::string &prefix)
	{
		this->_prefix = prefix;
		return *this;
	}

	const std::string sufix() const
	{
		return this->_sufix;
	}

	composed_uri &sufix(const std::string &sufix)
	{
		this->_sufix = sufix;
		return *this;
	}

	bool match(std::string requestUri, icarus::http::values<http::values_value> &params)
	{
		if (!this->regex)
			this->compile();

		if (this->regex)
		{
			boost::smatch results;

			if (boost::regex_match(requestUri, results, *this->regex))
			{
				for (regex_token &token : this->_tokens)
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
		for (regex_token &t : this->_tokens)
		{
			if (t.name().empty())
				stream << t.regex();
			else
				stream << "<" << t.name() << ":" << t.regex() << ">";
		}
		return stream.str();
	}

	unsigned int size()
	{
		return this->_tokens.size();
	}

	composed_uri &operator=(const composed_uri &uri)
	{
		this->_prefix = uri._prefix;
		this->_sufix = uri._sufix;
		for (const regex_token &token : uri._tokens)
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
enum class method_param_type
{
	NORMAL, REFERENCE, POINTER
};

class method_param
{
private:
	std::string _type;
	method_param_type _attribute;
	std::string _name;

public:

	method_param()
	{ }

	method_param(const std::string type, const method_param_type attribute, const std::string name)
		: _type(type), _attribute(attribute), _name(name)
	{ }

	const std::string &type() const
	{
		return this->_type;
	}

	method_param &type(const std::string &type)
	{
		this->_type = type;
		return *this;
	}

	const method_param_type &attribute() const
	{
		return this->_attribute;
	}

	method_param &attribute(const method_param_type &attr)
	{
		this->_attribute = attr;
		return *this;
	}

	const std::string &name() const
	{
		return this->_name;
	}

	method_param &name(const std::string &name)
	{
		this->_name = name;
		return *this;
	}

	method_param &operator=(const method_param &param)
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
class call_method
{
private:
	bool _static;

	std::vector<std::string> _path;
	std::string _name;

	std::vector<method_param> _params;
public:
	call_method()
		: _static(true)
	{ }

	call_method(const call_method &callMethod)
		: _static(callMethod._static), _path(callMethod._path), _name(callMethod._name), _params(callMethod._params)
	{ }

	const bool isStatic() const
	{
		return this->_static;
	}

	call_method &setStatic(bool _static)
	{
		this->_static = _static;
		return *this;
	}

	const std::vector<std::string> &path() const
	{
		return this->_path;
	}

	call_method &path(const std::string &path)
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

	call_method &name(const std::string &name)
	{
		this->_name = name;
		return *this;
	}

	std::vector<method_param> &params()
	{
		return this->_params;
	}

	call_method &add(std::string type, method_param_type attribute, std::string name)
	{
		this->_params.emplace_back(type, attribute, name);
		return *this;
	}

	std::string str()
	{
		std::string result;
		for (std::string &p : this->_path)
		{
			result += p;
		}
		result += "::";
		result += this->_name;
		result += "(";
		unsigned int i = 0;
		for (method_param &param : this->_params)
		{
			if (i++ > 0)
				result += ", ";
			result += param.type();
			result += " ";
			result += param.name();
		}
		result += ")";
		return result;
	}

	call_method &operator=(const call_method &method)
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
class piece
{
private:
	size_t _line;
public:
	piece(size_t line)
		: _line(line)
	{ }

	virtual ~piece()
	{ }

	size_t line() const
	{
		return this->_line;
	}

	void line(size_t line)
	{
		this->_line = line;
	}

	virtual piece *match(std::string method, std::string requestUri, http::values<http::values_value> &values) = 0;
};

class routes;

/**
 * Represents a route.
 */
class route
	: public piece
{
private:
	unsigned int _id;
	std::string _httpMethod;
	composed_uri _composedURI;
	call_method _callMethod;
public:
	route(icarus::routes::route &route)
		: piece(route.line()), _id(0), _httpMethod(route._httpMethod), _composedURI(route._composedURI),
		  _callMethod(route._callMethod)
	{ }

	route(size_t line)
		: piece(line), _id(0), _composedURI("^", "$")
	{ }

	route(size_t line, std::initializer_list<std::pair<std::string, std::string>> list)
		: piece(line), _id(0), _composedURI("^", "$")
	{
		for (auto &c : list)
		{
			this->uri().add(c.first, c.second);
		}
	}

	const unsigned int id() const
	{
		return this->_id;
	}

	route &id(unsigned int id)
	{
		this->_id = id;
		return *this;
	}

	const std::string &httpMethod() const
	{
		return this->_httpMethod;
	}

	route &httpMethod(const std::string &httpMethod)
	{
		this->_httpMethod = httpMethod;
		return *this;
	}

	composed_uri &uri()
	{
		return this->_composedURI;
	}

	call_method &callMethod()
	{
		return this->_callMethod;
	}

	route &callMethod(call_method &callMethod)
	{
		this->_callMethod = callMethod;
		return *this;
	}

	virtual piece *match(std::string method, std::string uri, http::values<http::values_value> &params) override
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

class routes
	: public piece
{
private:
	std::vector<std::unique_ptr<piece>> _pieces;
public:
	routes(size_t line)
		: piece(line)
	{ }

	const std::vector<std::unique_ptr<piece>> &pieces() const
	{
		return this->_pieces;
	}

	virtual piece *add(piece *piece)
	{
		this->_pieces.emplace_back(piece);
		return piece;
	}

	virtual piece *match(std::string method, std::string requestUri, http::values<http::values_value> &values)
	{
		LOG_TRACE("routes::match");
		piece *result = nullptr;
		for (const std::unique_ptr<piece> &piece : this->_pieces)
		{
			result = piece->match(method, requestUri, values);
			if (result)
				break;
		}
		return result;
	}
};

class Group
	: public routes
{
private:
	composed_uri _composedURI;
public:
	Group(size_t line)
		: routes(line), _composedURI("^", "")
	{ }

	composed_uri &uri()
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

class document
	: public routes
{
private:
	std::string _name;

	std::vector<std::string> _packages;
public:
	document(const std::string &name)
		: routes(0), _name(name)
	{ }

	const std::string &name()
	{
		return this->_name;
	}

	document &name(std::string name)
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
