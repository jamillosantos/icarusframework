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

	CallMethod& addParam(std::string type, MethodParam::MethodType attribute, std::string name)
	{
		this->_params.emplace_back(type, attribute, name);
		return *this;
	}
};

class RoutesLine
{
private:
	size_t line;

	std::string _httpMethod;
	std::vector<RegexToken> _regex;
	CallMethod _callMethod;
public:
	RoutesLine(size_t line)
		: line(line)
	{ }

	const std::string &httpMethod() const
	{
		return this->_httpMethod;
	}

	RoutesLine &httpMethod(const std::string &httpMethod)
	{
		this->_httpMethod = httpMethod;
		return *this;
	}

	const std::vector<RegexToken> &regex() const
	{
		return this->_regex;
	}

	RoutesLine& addToken(std::string name, std::string regex)
	{
		this->_regex.emplace_back(name, regex);
		return *this;
	}

	RoutesLine& addToken(std::string expression)
	{
		this->_regex.emplace_back(expression);
		return *this;
	}

	const CallMethod &callMethod() const
	{
		return this->_callMethod;
	}

	RoutesLine& callMethod(CallMethod &callMethod)
	{
		this->_callMethod = callMethod;
		return *this;
	}
};

class RoutesData
{
private:
	std::vector<RoutesLine> _lines;
public:
	const std::vector<RoutesLine> &lines() const
	{
		return this->_lines;
	}

	void addLine(RoutesLine &line)
	{
		this->_lines.push_back(line);
	}
};

}
}

#endif // ICARUSFRAMEWORK_ROUTES_DATA_HPP
