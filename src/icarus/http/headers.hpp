/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#ifndef ICARUSFRAMEWORK_HTTP_HEADERS_H
#define ICARUSFRAMEWORK_HTTP_HEADERS_H

#include <string>
#include <vector>

#include "../log.hpp"

namespace icarus
{
namespace http
{
class Value
{
private:
	std::string _name;
	std::string _value;
public:
	Value()
	{ }

	Value(const std::string &name, const std::string &value)
		: _name(name), _value(value)
	{ }

	const std::string &name()
	{
		return this->_name;
	}

	void name(const std::string &name)
	{
		this->_name = name;
	}

	const std::string &value()
	{
		return this->_value;
	}

	void value(const std::string &value)
	{
		this->_value = value;
	}
};

template <typename T>
class Values
{
public:
	virtual void set(const std::string &name, const std::string &value) = 0;

	virtual std::string get(const std::string &name) = 0;

	virtual void erase(const std::string &name) = 0;

	virtual void clear() = 0;

	virtual unsigned long size() = 0;
};

template <typename T>
class ValueList
	: public Values<T>
{
public:
	typedef typename std::vector<T>::iterator iterator;
private:
	std::vector<T> values;

	int indexOf(const std::string &name)
	{
		int i = 0;
		for (T &value : this->values)
		{
			if (value.name() == name)
				return i;
			i++;
		}
		return -1;
	}

public:
	virtual void set(const std::string &name, const std::string &value) override
	{
		int idx = this->indexOf(name);
		if (idx > 0)
			this->values[idx].value(value);
		else
			this->add(name, value);
	}

	virtual void erase(const std::string &name) override
	{
		int idx = this->indexOf(name);
		if (idx > 0)
			this->values.erase(this->values.begin() + idx);
	}

	virtual std::string get(const std::string &name) override
	{
		int idx = this->indexOf(name);
		if (idx > 0)
			return this->values[idx].value();
		return "";
	}

	virtual void add(std::string name, std::string value)
	{
		this->values.emplace_back(name, value);
	}

	virtual void clear() override
	{
		this->values.clear();
	}

	iterator begin()
	{
		return this->values.begin();
	}

	iterator end()
	{
		return this->values.end();
	}

	virtual unsigned long size() override
	{
		return this->values.size();
	}
};

template <typename T>
class ValuesHash
	: public Values<T>
{
	typedef typename std::map<std::string, T>::iterator iterator;
private:
	std::map<std::string, T> _container;
public:
	virtual void set(const std::string &name, const std::string &value) override
	{
		LOG_TRACE("ValuesHash::get - emplacing " << name << " with " << value);
		this->_container.emplace(std::make_pair(name, T(name, value)));
	}

	virtual std::string get(const std::string &name) override
	{
		auto search = this->_container.find(name);
		if(search == this->_container.end())
		{
			LOG_TRACE("ValuesHash::get - Could not find " << name);
			return "";
		}
		else
		{
			LOG_TRACE("ValuesHash::get - Found " << name);
			return search->second.value();
		}
	}

	virtual void erase(const std::string &name) override
	{
		this->_container.erase(name);
	}

	virtual void clear() override
	{
		this->_container.clear();
	}

	iterator begin()
	{
		return this->_container.begin();
	}

	iterator end()
	{
		return this->_container.end();
	}

	virtual unsigned long size() override
	{
		return this->_container.size();
	}
};
}
}

#endif //ICARUSFRAMEWORK_HTTP_HEADERS_H
