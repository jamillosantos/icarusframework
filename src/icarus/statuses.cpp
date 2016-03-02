/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#include <icarus/statuses.h>

icarus::status::status(int code, const std::string &value)
	: code(code), value(value)
{ }

icarus::status::status(const status &s)
	: code(s.code), value(s.value)
{ }

icarus::status icarus::statuses::OK(200, "OK");
icarus::status icarus::statuses::CREATED(201, "Created");
icarus::status icarus::statuses::ACCEPTED(202, "Accepted");
icarus::status icarus::statuses::NO_CONTENT(204, "No content");
icarus::status icarus::statuses::RESET_CONTENT(205, "Reset Content");
icarus::status icarus::statuses::PARTIAL_CONTENT(206, "Partial Content");
icarus::status icarus::statuses::MULTIPLE_CHOICES(300, "Multiple Choices");
icarus::status icarus::statuses::MOVE_PERMANENTLY(301, "Move Permanently");
icarus::status icarus::statuses::FOUND(302, "Found");
icarus::status icarus::statuses::SEE_OTHER(303, "See Other");
icarus::status icarus::statuses::NOT_MODIFIED(304, "Not Modified");
icarus::status icarus::statuses::TEMPORARY_REDIRECT(307, "Temporary Redirect");
icarus::status icarus::statuses::BAD_REQUEST(400, "Bad Request");
icarus::status icarus::statuses::UNAUTHORIZED(401, "Unauthorized");
icarus::status icarus::statuses::PAYMENT_REQUIRED(402, "Payment Required");
icarus::status icarus::statuses::FORBIDDEN(403, "Forbidden");
icarus::status icarus::statuses::NOT_FOUND(404, "Not Found");
icarus::status icarus::statuses::METHOD_NOT_ALLOWED(405, "Method Not Allowed");
icarus::status icarus::statuses::NOT_ACCEPTABLE(406, "Not Acceptable");
icarus::status icarus::statuses::REQUEST_TIMEOUT(408, "Request Timeout");
icarus::status icarus::statuses::CONFLICT(409, "Conflict");
icarus::status icarus::statuses::INTERNAL_ERROR(500, "Internal Error");
icarus::status icarus::statuses::NOT_IMPLEMENTED(501, "Not Implemented");
icarus::status icarus::statuses::BAD_GATEWAY(502, "Bad Gateway");
icarus::status icarus::statuses::SERVICE_UNAVAILABLE(503, "Service Unavailable");
icarus::status icarus::statuses::GATEWAY_TIMEOUT(504, "Gateway Timeout");
icarus::status icarus::statuses::VERSION_NOT_SUPORTED(50, "Version Not Suported");
