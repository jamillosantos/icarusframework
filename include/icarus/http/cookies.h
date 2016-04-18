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

typedef std::chrono::system_clock::time_point cookie_expiration_t;

class cookie_value
{
private:
	std::string _value;
	boost::optional<cookie_expiration_t> _expires;
public:
	cookie_value();

	cookie_value(const std::string &value);

	cookie_value(const std::string &value, const boost::optional<cookie_expiration_t> &expires);

	const std::string value() const;

	cookie_value& value(const std::string &value);

	const boost::optional<cookie_expiration_t>& expires() const;

	void expires(const cookie_expiration_t &date_time);
};

class cookies
{
public:
	typedef std::map<std::string, icarus::http::cookie_value> list;
	typedef std::pair<std::string, icarus::http::cookie_value> pair;
	typedef list::iterator iterator;
	typedef list::const_iterator const_iterator;
	typedef list::reverse_iterator reverse_iterator;
	typedef list::const_reverse_iterator const_reverse_iterator;
private:
	static boost::regex regex;
	list _list;
public:
	void parse(const std::string &cookies);

	const boost::optional<icarus::http::cookie_value> operator[](const std::string &name) const;

	void emplace(const std::string &name, const std::string &value);

	void set(const std::string &name, const std::string &value);

	void set(const std::string &name, const std::string &value, const icarus::http::cookie_expiration_t &expiration);

	iterator begin();
	iterator end();

	const_iterator begin() const;
	const_iterator end() const;

	const_iterator cbegin();
	const_iterator cend();

	reverse_iterator rbegin();
	reverse_iterator rend();

	const_reverse_iterator crbegin();
	const_reverse_iterator crend();

	size_t size() const;
};
}
}

#endif //ICARUSFRAMEWORK_HTTP_COOKIES_H
