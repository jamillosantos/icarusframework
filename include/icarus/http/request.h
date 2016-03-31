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
class request
{
protected:
	icarus::http::headers _headers;
	icarus::http::headers _serverVariables;

	std::string _contentType;
	long long int _contentLength;

	icarus::http::cookies _cookies;

	std::unique_ptr<std::istream> _content;

	std::string _uri;
	std::string _queryString;
	icarus::http::query_string_values _params;

	std::string _method;

	std::unique_ptr<Json::Value> _json;
public:
	request();

	virtual ~request();

	icarus::http::headers &headers();

	icarus::http::headers &serverVariables();

	std::istream &content();

	const std::string &contentType() const;

	const long long int &contentLength() const;

	icarus::http::cookies &cookies();

	const std::string &uri();

	const std::string &queryString();

	icarus::http::query_string_values &params();

	const std::string &method();

	const Json::Value &as_json();
};
}
}

#endif //ICARUSFRAMEWORK_REQUEST_H
