/**
 *
 * This conversion infrastructure is based on SOCI ideas.
 * @author J. Santos <jamillo@gmail.com>
 * @date March 12, 2016
 **/

#ifndef ICARUSFRAMEWORK_ROUTES_TYPECONVERSION_H
#define ICARUSFRAMEWORK_ROUTES_TYPECONVERSION_H

#include <string>

namespace icarus
{
template <typename T>
struct type_conversion
{
	typedef T base_type;

	static void from(base_type const &in, T &out)
	{
		out = in;
	}

	static void to(T const &in, base_type &out)
	{
		out = in;
	}
};

}

#endif //ICARUSFRAMEWORK_ROUTES_TYPECONVERSION_H
