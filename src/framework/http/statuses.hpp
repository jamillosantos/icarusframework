/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#ifndef ICARUSFRAMEWORK_HTTP_STATUSES_H
#define ICARUSFRAMEWORK_HTTP_STATUSES_H

#include <string>
#include "response.hpp"

namespace icarus
{
namespace http
{
class Status
{
public:
	Status(int code, const std::string &value) : code(code), value(value)
	{ }

	const int code;
	const std::string value;
};

namespace statuses
{
Status OK(200, "OK");
Status CREATED(201, "Created");
Status ACCEPTED(202, "Accepted");
Status NO_CONTENT(204, "No content");
Status RESET_CONTENT(205, "Reset Content");
Status PARTIAL_CONTENT(206, "Partial Content");
Status MULTIPLE_CHOICES(300, "Multiple Choices");
Status MOVE_PERMANENTLY(301, "Move Permanently");
Status FOUND(302, "Found");
Status SEE_OTHER(303, "See Other");
Status NOT_MODIFIED(304, "Not Modified");
Status TEMPORARY_REDIRECT(307, "Temporary Redirect");
Status BAD_REQUEST(400, "Bad Request");
Status UNAUTHORIZED(401, "Unauthorized");
Status PAYMENT_REQUIRED(402, "Payment Required");
Status FORBIDDEN(403, "Forbidden");
Status NOT_FOUND(404, "Not Found");
Status METHOD_NOT_ALLOWED(405, "Method Not Allowed");
Status NOT_ACCEPTABLE(406, "Not Acceptable");
Status REQUEST_TIMEOUT(408, "Request Timeout");
Status CONFLICT(409, "Conflict");
Status INTERNAL_ERROR(500, "Internal Error");
Status NOT_IMPLEMENTED(501, "Not Implemented");
Status BAD_GATEWAY(502, "Bad Gateway");
Status SERVICE_UNAVAILABLE(503, "Service Unavailable");
Status GATEWAY_TIMEOUT(504, "Gateway Timeout");
Status VERSION_NOT_SUPORTED(50, "Version Not Suported");
}
}
}

icarus::http::Response &operator<<(icarus::http::Response &response, const icarus::http::Status& status)
{
	response << status.code << " " << status.value;
	return *this;
}

#endif //ICARUSFRAMEWORK_HTTP_STATUSES_H
