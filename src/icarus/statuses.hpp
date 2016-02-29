/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#ifndef ICARUSFRAMEWORK_STATUSES_H
#define ICARUSFRAMEWORK_STATUSES_H

#include <string>

namespace icarus
{
class status
{
public:
	status(int code, const std::string &value)
		: code(code), value(value)
	{ }

	status(const status &s)
		: code(s.code), value(s.value)
	{ }

	int code;
	std::string value;
};

namespace statuses
{
status OK(200, "OK");
status CREATED(201, "Created");
status ACCEPTED(202, "Accepted");
status NO_CONTENT(204, "No content");
status RESET_CONTENT(205, "Reset Content");
status PARTIAL_CONTENT(206, "Partial Content");
status MULTIPLE_CHOICES(300, "Multiple Choices");
status MOVE_PERMANENTLY(301, "Move Permanently");
status FOUND(302, "Found");
status SEE_OTHER(303, "See Other");
status NOT_MODIFIED(304, "Not Modified");
status TEMPORARY_REDIRECT(307, "Temporary Redirect");
status BAD_REQUEST(400, "Bad Request");
status UNAUTHORIZED(401, "Unauthorized");
status PAYMENT_REQUIRED(402, "Payment Required");
status FORBIDDEN(403, "Forbidden");
status NOT_FOUND(404, "Not Found");
status METHOD_NOT_ALLOWED(405, "Method Not Allowed");
status NOT_ACCEPTABLE(406, "Not Acceptable");
status REQUEST_TIMEOUT(408, "Request Timeout");
status CONFLICT(409, "Conflict");
status INTERNAL_ERROR(500, "Internal Error");
status NOT_IMPLEMENTED(501, "Not Implemented");
status BAD_GATEWAY(502, "Bad Gateway");
status SERVICE_UNAVAILABLE(503, "Service Unavailable");
status GATEWAY_TIMEOUT(504, "Gateway Timeout");
status VERSION_NOT_SUPORTED(50, "Version Not Suported");
}
}

#endif //ICARUSFRAMEWORK_STATUSES_H
