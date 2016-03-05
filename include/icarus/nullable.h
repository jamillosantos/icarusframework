/**
 * @author J. Santos <jamillo@gmail.com>
 * @date March 04, 2016
 **/

#ifndef ICARUSFRAMEWORK_NULLABLE_H
#define ICARUSFRAMEWORK_NULLABLE_H

namespace icarus
{
template<typename T>
class nullable
{
private:
	T *_value;
	bool _null;
public:
	nullable()
		: _null(true)
	{ }

	nullable(T &t)
		: _value(&t), _null(false)
	{ }

	nullable(T *t)
		: _value(t), _null(t == nullptr)
	{ }

	nullable(const icarus::nullable<T> &n)
		: _value(n._value), _null(n._null)
	{ }

	bool null()
	{
		return this->_null;
	}

	operator bool() const
	{
		return (!this->_null);
	}

	T& operator*()
	{
		return *this->_value;
	}

	T& operator=(const T &value)
	{
		this->_value = &value;
		this->_null = (this->_value == nullptr);
		return *this;
	}

	T& operator=(const T *value)
	{
		this->_value = value;
		this->_null = (this->_value == nullptr);
		return *this;
	}
};
}


#endif //ICARUSFRAMEWORK_NULLABLE_H
