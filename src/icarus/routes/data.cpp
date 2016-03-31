/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#include <icarus/routes/data.h>

#include <icarus/log.h>

icarus::routes::regex_token::regex_token(const std::string &name, const std::string &regex)
	: _name(name), _regex(regex)
{ }

icarus::routes::regex_token::regex_token(const std::string &regex)
	: _name(""), _regex(regex)
{ }

const std::string &icarus::routes::regex_token::name() const
{
	return this->_name;
}

const std::string &icarus::routes::regex_token::regex() const
{
	return this->_regex;
}

icarus::routes::regex_token &icarus::routes::regex_token::regex(const std::string &regex)
{
	this->_regex = regex;
	return *this;
}

const int icarus::routes::regex_token::index() const
{
	return this->_index;
}

icarus::routes::regex_token &icarus::routes::regex_token::index(int index)
{
	this->_index = index;
	return *this;
}

void icarus::routes::composed_uri::compile()
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
	this->regex.reset(new boost::regex(pattern));
}

icarus::routes::composed_uri::composed_uri()
{ }

icarus::routes::composed_uri::composed_uri(const composed_uri &uri)
	: _tokens(uri._tokens), _prefix(uri._prefix), _sufix(uri._sufix)
{
	if (uri.regex)
		this->regex.reset(new boost::regex(*uri.regex));
}

icarus::routes::composed_uri::composed_uri(const std::string &prefix, const std::string &sufix)
	: _prefix(prefix), _sufix(sufix)
{ }

icarus::routes::composed_uri &icarus::routes::composed_uri::add(std::string name, std::string regex)
{
	this->_tokens.emplace_back(name, regex);
	this->regex.reset();
	return *this;
}

icarus::routes::composed_uri &icarus::routes::composed_uri::add(std::string expression)
{
	this->_tokens.emplace_back(expression);
	this->regex.reset();
	return *this;
}

std::vector<icarus::routes::regex_token> &icarus::routes::composed_uri::tokens()
{
	return this->_tokens;
}

const std::string icarus::routes::composed_uri::prefix() const
{
	return this->_prefix;
}

icarus::routes::composed_uri &icarus::routes::composed_uri::prefix(const std::string &prefix)
{
	this->_prefix = prefix;
	return *this;
}

const std::string icarus::routes::composed_uri::sufix() const
{
	return this->_sufix;
}

icarus::routes::composed_uri &icarus::routes::composed_uri::sufix(const std::string &sufix)
{
	this->_sufix = sufix;
	return *this;
}

bool icarus::routes::composed_uri::match(std::string requestUri, std::vector<std::string> &params)
{
	if (!this->regex)
		this->compile();

	if (this->regex)
	{
		boost::smatch results;

		if (boost::regex_match(requestUri, results, *this->regex))
		{
			for (icarus::routes::regex_token &token : this->_tokens)
			{
				if (!token.name().empty())
					params.emplace_back(results[token.index()]);
			}
			return true;
		}
	}
	return false;
}

std::string icarus::routes::composed_uri::str()
{
	std::stringstream stream;
	for (icarus::routes::regex_token &t : this->_tokens)
	{
		if (t.name().empty())
			stream << t.regex();
		else
			stream << "<" << t.name() << ":" << t.regex() << ">";
	}
	return stream.str();
}

unsigned int icarus::routes::composed_uri::size()
{
	return this->_tokens.size();
}

icarus::routes::composed_uri &icarus::routes::composed_uri::operator=(const icarus::routes::composed_uri &uri)
{
	this->_prefix = uri._prefix;
	this->_sufix = uri._sufix;
	for (const icarus::routes::regex_token &token : uri._tokens)
	{
		this->add(token.name(), token.regex());
	}
	if (uri.regex)
		this->regex.reset(new boost::regex(*uri.regex));
	else
		this->regex.reset();
	return *this;
}

icarus::routes::method_param::method_param()
{ }

icarus::routes::method_param::method_param(const std::string type, const method_param_type attribute, const std::string name)
	: _type(type), _attribute(attribute), _name(name)
{ }

const std::string &icarus::routes::method_param::type() const
{
	return this->_type;
}

icarus::routes::method_param &icarus::routes::method_param::type(const std::string &type)
{
	this->_type = type;
	return *this;
}

const icarus::routes::method_param_type &icarus::routes::method_param::attribute() const
{
	return this->_attribute;
}

icarus::routes::method_param &icarus::routes::method_param::attribute(const icarus::routes::method_param_type &attr)
{
	this->_attribute = attr;
	return *this;
}

const std::string &icarus::routes::method_param::name() const
{
	return this->_name;
}

icarus::routes::method_param &icarus::routes::method_param::name(const std::string &name)
{
	this->_name = name;
	return *this;
}

icarus::routes::method_param &icarus::routes::method_param::operator=(const icarus::routes::method_param &param)
{
	this->_attribute = param._attribute;
	this->_name = param._name;
	this->_type = param._type;
	return *this;
}

std::string icarus::routes::method_param::cpp()
{
	std::string result(this->_type);
	result += " ";
	if (this->_attribute == icarus::routes::method_param_type::POINTER)
		result += "*";
	else if (this->_attribute == icarus::routes::method_param_type::REFERENCE)
		result += "&";
	else if (this->_attribute == icarus::routes::method_param_type::RVALUE)
		result += "&&";
	result += this->_name;
	return result;
}

icarus::routes::call_method::call_method()
	: _static(true)
{ }

icarus::routes::call_method::call_method(const call_method &callMethod)
	: _static(callMethod._static), _path(callMethod._path), _name(callMethod._name), _params(callMethod._params)
{ }

const bool icarus::routes::call_method::isStatic() const
{
	return this->_static;
}

icarus::routes::call_method &icarus::routes::call_method::setStatic(bool _static)
{
	this->_static = _static;
	return *this;
}

const std::vector<std::string> &icarus::routes::call_method::path() const
{
	return this->_path;
}

icarus::routes::call_method &icarus::routes::call_method::path(const std::string &path)
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

const std::string &icarus::routes::call_method::name() const
{
	return this->_name;
}

icarus::routes::call_method &icarus::routes::call_method::name(const std::string &name)
{
	this->_name = name;
	return *this;
}

std::vector<icarus::routes::method_param> &icarus::routes::call_method::params()
{
	return this->_params;
}

icarus::routes::call_method &icarus::routes::call_method::add(std::string type, icarus::routes::method_param_type attribute, std::string name)
{
	this->_params.emplace_back(type, attribute, name);
	return *this;
}

std::string icarus::routes::call_method::str()
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

icarus::routes::call_method &icarus::routes::call_method::operator=(const icarus::routes::call_method &method)
{
	this->_name = method._name;
	this->_params = method._params;			// TODO
	this->_path = method._path;				// TODO
	this->_static = method._static;
	return *this;
}

icarus::routes::piece::piece(size_t line)
	: _line(line)
{ }

icarus::routes::piece::~piece()
{ }

size_t icarus::routes::piece::line() const
{
	return this->_line;
}

void icarus::routes::piece::line(size_t line)
{
	this->_line = line;
}

icarus::routes::route::route(icarus::routes::route &route)
	: piece(route.line()), _id(0), _http_method(route._http_method), _composed_uri(route._composed_uri),
	  _call_method(route._call_method)
{ }

icarus::routes::route::route(size_t line)
	: piece(line), _id(0), _composed_uri("^", "$")
{ }

icarus::routes::route::route(size_t line, const std::string &http_method, std::initializer_list<std::pair<std::string, std::string>> list)
	: piece(line), _id(0), _http_method(http_method), _composed_uri("^", "$")
{
	for (auto &c : list)
	{
		this->uri().add(c.first, c.second);
	}
}

const unsigned int icarus::routes::route::id() const
{
	return this->_id;
}

icarus::routes::route &icarus::routes::route::id(unsigned int id)
{
	this->_id = id;
	return *this;
}

const std::string &icarus::routes::route::http_method() const
{
	return this->_http_method;
}

icarus::routes::route &icarus::routes::route::http_method(const std::string &httpMethod)
{
	this->_http_method = httpMethod;
	return *this;
}

icarus::routes::composed_uri &icarus::routes::route::uri()
{
	return this->_composed_uri;
}

icarus::routes::call_method &icarus::routes::route::call_method()
{
	return this->_call_method;
}

icarus::routes::route &icarus::routes::route::call_method(icarus::routes::call_method &call_method)
{
	this->_call_method = call_method;
	return *this;
}

icarus::routes::piece *icarus::routes::route::match(std::string method, std::string uri,
	std::vector<std::string> &params)
{
	if (
		(this->http_method() == method)
		|| (
			(this->http_method() == "WS")
			&& (method == "GET")
		)
	)
	{
		if (this->uri().match(uri, params))
			return this;
	}
	return nullptr;
}

icarus::routes::routes::routes(size_t line)
	: piece(line)
{ }

const std::vector<std::unique_ptr<icarus::routes::piece>> &icarus::routes::routes::pieces() const
{
	return this->_pieces;
}

icarus::routes::piece *icarus::routes::routes::add(icarus::routes::piece *piece)
{
	this->_pieces.emplace_back(piece);
	return piece;
}

icarus::routes::piece *icarus::routes::routes::match(std::string method, std::string requestUri,
	std::vector<std::string> &values)
{
	LOG_TRACE("routes::match");
	icarus::routes::piece *result = nullptr;
	for (const std::unique_ptr<icarus::routes::piece> &piece : this->_pieces)
	{
		result = piece->match(method, requestUri, values);
		if (result)
			break;
	}
	return result;
}

icarus::routes::group::group(size_t line)
	: routes(line), _composed_uri("^", "")
{ }

icarus::routes::composed_uri &icarus::routes::group::uri()
{
	return this->_composed_uri;
}

icarus::routes::document::document(const std::string &name)
	: routes(0), _name(name)
{ }

const std::string &icarus::routes::document::name()
{
	return this->_name;
}

icarus::routes::document &icarus::routes::document::name(std::string name)
{
	this->_name = name;
	return *this;
}

std::vector<std::string> &icarus::routes::document::packages()
{
	return this->_packages;
}
