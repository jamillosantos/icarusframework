/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 12, 2016
 **/

#ifndef ICARUSFRAMEWORK_REQUEST_H
#define ICARUSFRAMEWORK_REQUEST_H

#include <icarus/http/headers.h>
#include <icarus/http/cookies.h>
#include <icarus/http/querystring.h>
#include <json/value.h>

namespace icarus
{
namespace http
{
class irequest
{
public:
	virtual std::string header(const std::string &name) = 0;

	virtual std::string env(const std::string &name) = 0;

	virtual std::unique_ptr<std::istream> &content() = 0;

	virtual const std::string &content_type() const = 0;

	virtual const size_t &content_length() const = 0;

	virtual const std::string cookie(const std::string &name) const = 0;

	virtual const std::string &uri() const = 0;
	virtual const std::string &query_string() const = 0;

	virtual const std::string &param(const std::string &name) = 0;
	virtual const icarus::http::query_string_value &params(const std::string &name) = 0;

	virtual const std::string &method() = 0;

	virtual const Json::Value &as_json() = 0;
	virtual const bool is_json() const = 0;
};
}
}

#endif //ICARUSFRAMEWORK_REQUEST_H
