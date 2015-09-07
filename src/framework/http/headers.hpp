/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#ifndef ICARUSFRAMEWORK_HTTP_HEADERS_H
#define ICARUSFRAMEWORK_HTTP_HEADERS_H

#include <string>
#include <vector>

namespace icarus
{
namespace http
{
class Header
{
public:
	Header()
	{ }

	Header(const std::string &name, const std::string &value) : name(name), value(value)
	{ }

	std::string name;
	std::string value;
};

class Headers
{
private:
	std::vector<Header> headers;

	int indexOf(std::string &name)
	{
		int i = 0;
		for (Header &header : this->headers)
		{
			if (header.name == name)
				return i;
			i++;
		}
		return -1;
	}

public:
	virtual void set(std::string name, std::string value)
	{
		int idx = this->indexOf(name);
		if (idx > 0)
			this->headers[idx].value = value;
		else
			this->add(name, value);
	}

	virtual void add(std::string name, std::string value)
	{
		this->headers.emplace_back(name, value);
	}

	virtual void erase(std::string name)
	{
		int idx = this->indexOf(name);
		if (idx > 0)
			this->headers.erase(this->headers.begin() + idx);
	}

	virtual std::string get(std::string name)
	{
		int idx = this->indexOf(name);
		if (idx > 0)
			return this->headers[idx].value;
		return "";
	}

	std::vector<icarus::http::Header>::iterator begin()
	{
		return this->headers.begin();
	}

	std::vector<icarus::http::Header>::iterator end()
	{
		return this->headers.end();
	}
};
}
}

#endif //ICARUSFRAMEWORK_HTTP_HEADERS_H
