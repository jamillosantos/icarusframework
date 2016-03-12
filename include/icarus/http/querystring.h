/**
 * @author J. Santos <jamillo@gmail.com>
 * @date March 08, 2016
 **/

#ifndef ICARUSFRAMEWORK_QUERYSTRING_H
#define ICARUSFRAMEWORK_QUERYSTRING_H

#include <string>
#include <memory>
#include <boost/regex.hpp>
#include "headers.h"

namespace icarus
{
namespace http
{

class query_string_value
{
public:
	typedef typename std::vector<std::string>::iterator iterator;
	typedef typename std::vector<std::string>::reverse_iterator reverse_iterator;
private:
	std::vector<std::string> _values;
public:

	const std::string & operator*();

	const std::string & value();

	const std::string & value_at(unsigned int index);

	const std::string & operator[](const unsigned int index);

	const bool empty();

	void add(const std::string &value);

	void clear();

	size_t size();

	iterator begin();

	iterator end();

	reverse_iterator rbegin();

	reverse_iterator rend();
};

class query_string_values
{
public:
	typedef typename std::map<std::string, std::unique_ptr<icarus::http::query_string_value>>::iterator iterator;
	typedef typename std::map<std::string, std::unique_ptr<icarus::http::query_string_value>>::const_iterator const_iterator;
	typedef typename std::map<std::string, std::unique_ptr<icarus::http::query_string_value>>::reverse_iterator reverse_iterator;
	typedef typename std::map<std::string, std::unique_ptr<icarus::http::query_string_value>>::const_reverse_iterator const_reverse_iterator;
private:
	std::map<std::string, std::unique_ptr<icarus::http::query_string_value>> _values;
public:
	virtual icarus::http::query_string_value& at(const std::string &name);

	icarus::http::query_string_value& operator[](const std::string &name);

	icarus::http::query_string_values& add(const std::string &name, const std::string &value);

	iterator begin();
	const_iterator cbegin();

	iterator end();
	const_iterator cend();

	reverse_iterator rbegin();
	const_reverse_iterator crbegin();

	reverse_iterator rend();
	const_reverse_iterator crend();
};

class query_string
{
public:
	static void parse(const std::string &query, icarus::http::query_string_values &values);
};
}
}


#endif //ICARUSFRAMEWORK_QUERYSTRING_H
