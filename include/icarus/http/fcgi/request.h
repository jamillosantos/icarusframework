/**
* @author J. Santos <jamillo@gmail.com>
* @date January 13, 2016
**/

#ifndef ICARUSFRAMEWORK_FCGI_REQUEST_H
#define ICARUSFRAMEWORK_FCGI_REQUEST_H

#include <icarus/http/request.h>
#include <icarus/exceptions.h>
#include <icarus/http/cookies.h>

#include <fcgiapp.h>
#include <memory>

namespace icarus
{
namespace http
{
namespace fcgi
{
class request
	: public icarus::http::irequest
{
private:
	icarus::http::headers _headers;
	size_t _content_length;
	icarus::http::cookies _cookies;
	icarus::http::headers _env;
	std::string _content_type;
	std::string _uri;
	std::string _query_string;
	std::string _method;
	icarus::http::query_string_values _query_string_values;
	std::unique_ptr<Json::Value> _json;
	std::unique_ptr<std::istream> _content;
public:
	virtual void init(const FCGX_Request &request);

	virtual std::string header(const std::string &name) override;

	virtual std::string env(const std::string &name) override;

	virtual std::unique_ptr<std::istream> &content() override;

	virtual const std::string & content_type() const override;

	virtual const size_t & content_length() const override;

	virtual const std::string cookie(const std::string &name) const override;

	virtual const std::string &uri() const override;

	virtual const std::string &query_string() const override;

	virtual const std::string &param(const std::string &name) override;

	virtual const icarus::http::query_string_value &params(const std::string &name) override;

	virtual const std::string &method() override;

	virtual const Json::Value &as_json() override;

	virtual const bool is_json() const override;

	template<class T>
	icarus::http::fcgi::request &operator>>(T& t)
	{
		(*this->_content) >> t;
		return *this;
	}
};
}
}
}

#endif //ICARUSFRAMEWORK_FCGI_REQUEST_H
