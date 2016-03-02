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
	status(int code, const std::string &value);

	status(const status &s);

	int code;
	std::string value;
};

class statuses
{
public:
	static icarus::status OK;
	static icarus::status CREATED;
	static icarus::status ACCEPTED;
	static icarus::status NO_CONTENT;
	static icarus::status RESET_CONTENT;
	static icarus::status PARTIAL_CONTENT;
	static icarus::status MULTIPLE_CHOICES;
	static icarus::status MOVE_PERMANENTLY;
	static icarus::status FOUND;
	static icarus::status SEE_OTHER;
	static icarus::status NOT_MODIFIED;
	static icarus::status TEMPORARY_REDIRECT;
	static icarus::status BAD_REQUEST;
	static icarus::status UNAUTHORIZED;
	static icarus::status PAYMENT_REQUIRED;
	static icarus::status FORBIDDEN;
	static icarus::status NOT_FOUND;
	static icarus::status METHOD_NOT_ALLOWED;
	static icarus::status NOT_ACCEPTABLE;
	static icarus::status REQUEST_TIMEOUT;
	static icarus::status CONFLICT;
	static icarus::status INTERNAL_ERROR;
	static icarus::status NOT_IMPLEMENTED;
	static icarus::status BAD_GATEWAY;
	static icarus::status SERVICE_UNAVAILABLE;
	static icarus::status GATEWAY_TIMEOUT;
	static icarus::status VERSION_NOT_SUPORTED;
};
}

#endif //ICARUSFRAMEWORK_STATUSES_H
