/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#ifndef ICARUSFRAMEWORK_RESULT_H
#define ICARUSFRAMEWORK_RESULT_H

#include <boost/optional/optional.hpp>

#include <sstream>

#include <icarus/log.h>
#include <icarus/http/headers.h>
#include <icarus/content.h>
#include <icarus/statuses.h>

#include <json/value.h>

namespace icarus
{
class result
	: public content
{
private:
	icarus::status &_status;

	boost::optional<std::string> _contentType;
	boost::optional<std::string> _charset;

	icarus::http::value_list<icarus::http::values_value> _headers;
public:
	result();

	result(icarus::status &status);

	result(icarus::status &status, const std::string &content);

	result(const std::string &content);

	result(icarus::status &status, const Json::Value &json);

	result(const Json::Value &json);

	result(icarus::result &&r);

	const icarus::http::value_list<icarus::http::values_value> &headers() const;

	result &status(icarus::status &status);

	icarus::status &status() const;

	result &charset(std::string &charset);

	boost::optional<std::string> charset() const;

	result &as(std::string &contentType);

	boost::optional<std::string> contentType() const;

	icarus::result &header(const std::string &name, const std::string &value);
};

namespace results
{
static result status(icarus::status &status, const std::string &content);

static result &status(icarus::result &result, icarus::status &status);

static result ok(const std::string &content);

static result ok(const Json::Value &json);

static icarus::result &&ok(icarus::result &result);

static result notImplemented(const std::string &content);

static result &notImplemented(icarus::result &result);

static result notImplemented(const Json::Value &json);

static result badRequest(const std::string &content);

static result &badRequest(icarus::result &result);

static result badRequest(const Json::Value &json);

static result unauthorized(const std::string &content);

static result &unauthorized(icarus::result &result);

static result unauthorized(const Json::Value &json);

static result paymentRequired(const std::string &content);

static result &paymentRequired(icarus::result &result);

static result paymentRequired(const Json::Value &json);

static result forbidden(const std::string &content);

static result &forbidden(icarus::result &result);

static result forbidden(const Json::Value &json);

static result notFound(const std::string &content);

static result &notFound(icarus::result &result);

static result notFound(const Json::Value &json);

static result internalServerError(const std::string &content);

static result &internalServerError(icarus::result &result);

static result internalServerError(const Json::Value &json);

static result movedPermanently(std::string url);

static result redirect(std::string url);

static result temporaryRedirect(std::string url);

}
}

std::ostream &operator<<(std::ostream &out, icarus::result &result);

#endif // ICARUSFRAMEWORK_RESULT_H
