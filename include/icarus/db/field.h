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
	field();

	field(const T& value);

	inline T& value();

	inline icarus::db::field<T> &value(icarus::db::field<T> &field, const T &value);

	inline T& old_value();

	bool is_modified();

	bool is_null();

	icarus::db::field<T> &clear();
};
}
}

#endif //ICARUSFRAMEWORK_DB_FIELD_HPP
