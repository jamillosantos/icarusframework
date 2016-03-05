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

class cookie
	: public values_value
{
private:
	boost::optional<std::chrono::system_clock::time_point> _expires;
public:
	cookie();

	cookie(const std::string &name, const std::string &value);

	cookie(const std::string &name, const std::string &value, const boost::optional<std::chrono::system_clock::time_point> &expires);

	const boost::optional<std::chrono::system_clock::time_point>& expires() const;

	void expires(const std::chrono::system_clock::time_point &dateTime);
};

class cookies
	: public value_hash<cookie>
{
private:
	static boost::regex regex;
public:
	void parse(const std::string &cookies);
};
}
}

#endif //ICARUSFRAMEWORK_HTTP_COOKIES_H
