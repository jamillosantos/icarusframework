/**
 * @author J. Santos <jamillo@gmail.com>
 * @date March 08, 2016
 **/

#include <icarus/http/querystring.h>
#include <icarus/general.h>
#include <iostream>

const std::string &icarus::http::query_string_value::operator*()
{
	if (this->_values.empty())
		return icarus::emptyString;
	else
		return this->_values[0];
}

const std::string &icarus::http::query_string_value::value()
{
	if (this->_values.empty())
		return icarus::emptyString;
	else
		return this->_values[0];
}

const std::string &icarus::http::query_string_value::value_at(unsigned int index)
{
	return this->_values[index];
}

const std::string &icarus::http::query_string_value::operator[](const unsigned int index)
{
	return this->_values[index];
}

const bool icarus::http::query_string_value::empty()
{
	return this->_values.empty();
}

void icarus::http::query_string_value::add(const std::string &value)
{
	this->_values.push_back(value);
}

void icarus::http::query_string_value::clear()
{
	this->_values.clear();
}

size_t icarus::http::query_string_value::size()
{
	return this->_values.size();
}

icarus::http::query_string_value::iterator icarus::http::query_string_value::begin()
{
	return this->_values.begin();
}

icarus::http::query_string_value::iterator icarus::http::query_string_value::end()
{
	return this->_values.end();
}

icarus::http::query_string_value::reverse_iterator icarus::http::query_string_value::rbegin()
{
	return this->_values.rbegin();
}

icarus::http::query_string_value::reverse_iterator icarus::http::query_string_value::rend()
{
	return this->_values.rend();
}

icarus::http::query_string_value &icarus::http::query_string_values::at(const std::string &name)
{
	icarus::http::query_string_values::iterator f = this->_values.find(name);
	if (f == this->_values.end())
	{
		icarus::http::query_string_value *v = new icarus::http::query_string_value();
		this->_values.emplace(name, std::unique_ptr<icarus::http::query_string_value>(v));
		return *v;
	}
	else
		return *(f->second);
}

icarus::http::query_string_value &icarus::http::query_string_values::operator[](const std::string &name)
{
	icarus::http::query_string_values::iterator f = this->_values.find(name);
	if (f == this->_values.end())
	{
		icarus::http::query_string_value *v = new icarus::http::query_string_value();
		this->_values.emplace(name, std::unique_ptr<icarus::http::query_string_value>(v));
		return *v;
	}
	else
		return *(f->second);
}

icarus::http::query_string_values &icarus::http::query_string_values::add(const std::string &name, const std::string &value)
{
	icarus::http::query_string_values::iterator f = this->_values.find(name);
	if (f == this->_values.end())
	{
		icarus::http::query_string_value *v = new icarus::http::query_string_value();
		this->_values.emplace(name, std::unique_ptr<icarus::http::query_string_value>(v));
		v->add(value);
	}
	else
		f->second->add(value);
	return *this;
}

icarus::http::query_string_values::iterator icarus::http::query_string_values::begin()
{
	return this->_values.begin();
}

icarus::http::query_string_values::const_iterator icarus::http::query_string_values::cbegin()
{
	return this->_values.cbegin();
}

icarus::http::query_string_values::iterator icarus::http::query_string_values::end()
{
	return this->_values.end();
}

icarus::http::query_string_values::const_iterator icarus::http::query_string_values::cend()
{
	return this->_values.cend();
}

icarus::http::query_string_values::reverse_iterator icarus::http::query_string_values::rbegin()
{
	return this->_values.rbegin();
}

icarus::http::query_string_values::const_reverse_iterator icarus::http::query_string_values::crbegin()
{
	return this->_values.crbegin();
}

icarus::http::query_string_values::reverse_iterator icarus::http::query_string_values::rend()
{
	return this->_values.rend();
}

icarus::http::query_string_values::const_reverse_iterator icarus::http::query_string_values::crend()
{
	return this->_values.crend();
}

void icarus::http::query_string::parse(const std::string &query, icarus::http::query_string_values &values)
{
	unsigned int
		checkpoint;
	std::string name;
	for (unsigned i = 0; i < query.size(); ++i)
	{
		checkpoint = i;
		for (; i < query.size(); ++i)
		{
			if (query[i] == '=')
			{
				name = query.substr(checkpoint, (i - checkpoint));
				checkpoint = ++i;
				for (; (i < query.size()) && (query[i] != '&'); ++i)
				{ }
				values[name].add(query.substr(checkpoint, i - checkpoint));
				break;
			}
		}
	}
}
