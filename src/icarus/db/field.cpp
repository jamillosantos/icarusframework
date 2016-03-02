/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 24, 2016
 **/

#ifndef ICARUSFRAMEWORK_DB_FIELD_HPP
#define ICARUSFRAMEWORK_DB_FIELD_HPP

#include <boost/optional.hpp>

namespace icarus
{
namespace db
{
template <typename T>
class field
{
private:
	boost::optional<T> _value;
	boost::optional<T> _old_value;

	bool _modified;
public:
	field()
		: _modified(false)
	{ }

	field(const T& value)
		: _modified(false), _value(value)
	{ }

	inline T& value()
	{
		return this->_value;
	}

	inline icarus::db::field<T> &value(icarus::db::field<T> &field, const T &value)
	{
		if (this->_value != value)
		{
			this->_old_value = this->_value;
			this->_value = value;
			this->_modified = true;
		}
		return *this;
	}

	inline T& old_value()
	{
		return this->_old_value;
	}

	bool is_modified()
	{
		return this->_modified;
	}

	bool is_null()
	{
		return this->_value;
	}

	icarus::db::field<T> &clear()
	{
		if (this->_value)
		{
			this->_old_value.reset(this->_value.get());
			this->_value.reset();
			this->_modified = true;
		}
		return *this;
	}
};
}
}

#endif //ICARUSFRAMEWORK_DB_FIELD_HPP
