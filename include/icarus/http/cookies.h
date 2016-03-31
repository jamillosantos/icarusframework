/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 14, 2016
 **/

#ifndef ICARUSFRAMEWORK_HTTP_COOKIES_H
#define ICARUSFRAMEWORK_HTTP_COOKIES_H

#include <boost/regex.hpp>
#include <boost/concept_check.hpp>
#include <boost/log/trivial.hpp>
#include <chrono>
#include <iomanip>
#include <boost/optional/optional.hpp>

#include "headers.h"
#include "icarus/log.h"
#include "icarus/exceptions.h"

#include <icarus/exceptions.h>

namespace icarus
{
namespace http
{
class cookies_parser_exception
	: public icarus::exception
{
public:
	cookies_parser_exception(std::exception *cause);

	cookies_parser_exception();
};

class cookie_value
{
private:
	std::string _value;
	boost::optional<std::chrono::system_clock::time_point> _expires;
public:
	cookie_value();

	cookie_value(const std::string &value);

	cookie_value(const std::string &value, const boost::optional<std::chrono::system_clock::time_point> &expires);

	const boost::optional<std::chrono::system_clock::time_point>& expires() const;

	void expires(const std::chrono::system_clock::time_point &dateTime);
};

class cookies
{
public:
	typedef std::map<std::string, icarus::http::cookie_value> list;
	typedef list::iterator iterator;
	typedef list::const_iterator const_iterator;
	typedef list::reverse_iterator reverse_iterator;
	typedef list::const_reverse_iterator const_reverse_iterator;
private:
	static boost::regex regex;
	list _list;
public:
	void parse(const std::string &cookies);

	const boost::optional<cookie_value&> operator[](const std::string &name);

	void emplace(const std::string &name, const std::string &value);

	iterator begin();
	iterator end();

	const_iterator cbegin();
	const_iterator cend();

	reverse_iterator rbegin();
	reverse_iterator rend();

	const_reverse_iterator crbegin();
	const_reverse_iterator crend();
};
}
}

#endif //ICARUSFRAMEWORK_HTTP_COOKIES_H
