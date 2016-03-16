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
	result(icarus::status &status);

	result(icarus::status &status, const std::string &content);

	result(const std::string &content);

	result(icarus::result &&r);

	icarus::http::value_list<icarus::http::values_value> &headers();

	result &status(icarus::status &status);

	icarus::status &status() const;

	result &charset(std::string &charset);

	boost::optional<std::string> charset() const;

	result &as(std::string &contentType);

	boost::optional<std::string> contentType() const;
};

namespace results
{
static result status(icarus::status &status, const std::string &content);

static result ok(const std::string &content);

static result notImplemented(const std::string &content);

static result badRequest(const std::string &content);

static result unauthorized(const std::string &content);

static result paymentRequired(const std::string &content);

static result forbidden(const std::string &content);

static result notFound(const std::string &content);

static result internalServerError(const std::string &content);

static result movedPermanently(std::string url);

static result redirect(std::string url);

static result temporaryRedirect(std::string url);
}
}

std::ostream &operator<<(std::ostream &out, icarus::result &result);

#endif // ICARUSFRAMEWORK_RESULT_H
