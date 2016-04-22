/**
* @author J. Santos <jamillo@gmail.com>
* @date January 13, 2016
**/

#include <icarus/http/fcgi/request.h>

#include <fcgio.h>
#include <json/reader.h>

void icarus::http::fcgi::request::init(const FCGX_Request &request)
{
	this->_content_length = -1;

	std::string
		header,
		headerName,
		headerValue;

	size_t fr;
	for (char **p = request.envp; *p; ++p)
	{
		header = *p;
		fr = header.find('=');
		if (fr != std::string::npos)
		{
			headerName = header.substr(0, fr);
			headerValue = header.substr(fr + 1, header.length() - fr - 1);
			if (headerName == "HTTP_COOKIE")
			{
				this->_cookies.parse(headerValue);
			}
			else if (headerName.compare(0, 5, "HTTP_") == 0)
			{
				this->_headers.emplace(
					headerName.substr(5, headerName.length() - 5),
					headerValue
				);
			}
			else
			{
				this->_env.emplace(
					headerName,
					headerValue
				);
				if (headerName == "CONTENT_TYPE")
					this->_content_type = headerValue;
				else if (headerName == "DOCUMENT_URI")
				{
					this->_uri = headerValue;
				}
				else if (headerName == "QUERY_STRING")
				{
					this->_query_string = headerValue;
					icarus::http::query_string::parse(headerValue, this->_query_string_values);
				}
				else if (headerName == "REQUEST_METHOD")
				{
					this->_method = headerValue;
				}
				else if (headerName == "CONTENT_LENGTH")
				{
					char *estr;
					this->_content_length = std::strtol(headerValue.c_str(), &estr, 10);
					if (*estr)
					{
						this->_content_length = -1;
					}
				}
			}
		}
		else
		{
			LOG_ERROR("Malformed header: " << header);
		}
	}
	this->_content.reset(new fcgi_istream(request.in));
}

std::string icarus::http::fcgi::request::header(const std::string &name)
{
	return this->_headers[name]->value();
}

std::string icarus::http::fcgi::request::env(const std::string &name)
{
	const auto &value = this->_env[name];
	if (value)
		return value->value();
	else
		return "";
}

std::unique_ptr<std::istream> &icarus::http::fcgi::request::content()
{
	return this->_content;
}

const std::string &icarus::http::fcgi::request::content_type() const
{
	return this->_content_type;
}

const size_t &icarus::http::fcgi::request::content_length() const
{
	return this->_content_length;
}

const std::string icarus::http::fcgi::request::cookie(const std::string &name) const
{
	const boost::optional<icarus::http::cookie_value> cookie = this->_cookies[name];
	if (cookie)
		return cookie->value();
	else
		return "";
}

const std::string &icarus::http::fcgi::request::uri() const
{
	return this->_uri;
}

const std::string &icarus::http::fcgi::request::query_string() const
{
	return this->_query_string;
}

const std::string &icarus::http::fcgi::request::param(const std::string &name)
{
	return this->_query_string_values["name"].value();
}

const icarus::http::query_string_value &icarus::http::fcgi::request::params(const std::string &name)
{
	return this->_query_string_values[name];
}

const std::string &icarus::http::fcgi::request::method()
{
	return this->_method;
}

const Json::Value &icarus::http::fcgi::request::as_json()
{
	if (this->_json)
		return *this->_json;
	else if (this->is_json())
	{
		std::string json_content;
		char data[2048];
		while (!this->content()->eof())
		{
			this->content()->read(data, 2048);
			json_content.append(data, this->content()->gcount());
		}
		Json::Reader reader;
		this->_json.reset(new Json::Value());
		reader.parse(json_content, *this->_json, false);
		return *this->_json;
	}
	else
		throw icarus::json_expected_exception();
}

const bool icarus::http::fcgi::request::is_json() const
{
	return (this->_content_type == "application/json");
}