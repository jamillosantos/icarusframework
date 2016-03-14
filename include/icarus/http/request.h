/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 12, 2016
 **/

#ifndef ICARUSFRAMEWORK_REQUEST_H
#define ICARUSFRAMEWORK_REQUEST_H

#include <icarus/http/headers.h>
#include <icarus/http/cookies.h>
#include <icarus/http/querystring.h>

namespace icarus
{
namespace http
{
class request
{
protected:
	value_list<values_value> _headers;
	value_list<values_value> _serverVariables;

	std::string _contentType;
	long long int _contentLength;

	icarus::http::cookies _cookies;

	std::unique_ptr<std::istream> _content;

	std::string _uri;
	std::string _queryString;
	icarus::http::query_string_values _params;

	std::string _method;
public:
	request();

	virtual ~request();

	value_list<values_value> &headers();

	value_list<values_value> &serverVariables();

	std::istream &content();

	const std::string &contentType() const;

	const long long int &contentLength() const;

	icarus::http::cookies &cookies();

	const std::string &uri();

	const std::string &queryString();

	icarus::http::query_string_values &params();

	const std::string &method();
};
}
}

#endif //ICARUSFRAMEWORK_REQUEST_H
