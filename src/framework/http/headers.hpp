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
class Value
{
public:
	Value()
	{ }

	Value(const std::string &name, const std::string &value) : name(name), value(value)
	{ }

	std::string name;
	std::string value;
};

class Values
{
private:
	std::vector<Value> values;

	int indexOf(std::string &name)
	{
		int i = 0;
		for (Value &value : this->values)
		{
			if (value.name == name)
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
			this->values[idx].value = value;
		else
			this->add(name, value);
	}

	virtual void add(std::string name, std::string value)
	{
		this->values.emplace_back(name, value);
	}

	virtual void erase(std::string name)
	{
		int idx = this->indexOf(name);
		if (idx > 0)
			this->values.erase(this->values.begin() + idx);
	}

	virtual std::string get(std::string name)
	{
		int idx = this->indexOf(name);
		if (idx > 0)
			return this->values[idx].value;
		return "";
	}

	std::vector<icarus::http::Value>::iterator begin()
	{
		return this->values.begin();
	}

	std::vector<icarus::http::Value>::iterator end()
	{
		return this->values.end();
	}
};
}
}

#endif //ICARUSFRAMEWORK_HTTP_HEADERS_H
