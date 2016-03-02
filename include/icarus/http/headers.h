/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#ifndef ICARUSFRAMEWORK_HTTP_HEADERS_H
#define ICARUSFRAMEWORK_HTTP_HEADERS_H

#include <string>
#include <vector>
#include <map>

namespace icarus
{
namespace http
{
class values_value
{
private:
	std::string _name;
	std::string _value;
public:
	values_value();

	values_value(const std::string &name, const std::string &value);

	const std::string &name();

	void name(const std::string &name);

	const std::string &value();

	void value(const std::string &value);
};

template <typename T>
class values
{
public:
	virtual void set(const std::string &name, const std::string &value) = 0;

	virtual std::string get(const std::string &name) = 0;

	virtual std::string get(const unsigned int index) = 0;

	virtual void erase(const std::string &name) = 0;

	virtual void clear() = 0;

	virtual unsigned long size() = 0;
};

template <typename T>
class value_list
	: public values<T>
{
public:
	typedef typename std::vector<T>::iterator iterator;
private:
	std::vector<T> values;

	int index_of(const std::string &name)
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
		int idx = this->index_of(name);
		if (idx > 0)
			this->values[idx].value(value);
		else
			this->add(name, value);
	}

	virtual void erase(const std::string &name) override
	{
		int idx = this->index_of(name);
		if (idx >= 0)
			this->values.erase(this->values.begin() + idx);
	}

	virtual std::string get(const std::string &name) override
	{
		int idx = this->index_of(name);
		if (idx >= 0)
			return this->values[idx].value();
		return "";
	}

	virtual std::string get(const unsigned int index) override
	{
		return this->values[index].value();
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
class value_hash
	: public values<T>
{
	typedef typename std::map<std::string, T>::iterator iterator;
private:
	std::map<std::string, T> _container;
public:
	virtual void set(const std::string &name, const std::string &value) override
	{
		this->_container.emplace(std::make_pair(name, T(name, value)));
	}

	virtual std::string get(const std::string &name) override
	{
		auto search = this->_container.find(name);
		if(search == this->_container.end())
			return "";
		else
			return search->second.value();
	}

	virtual std::string get(const unsigned int index) override
	{
		auto b = this->_container.begin();
		for (unsigned int i = 0; i < index; i++)
			++b;
		return b->second.value();
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
