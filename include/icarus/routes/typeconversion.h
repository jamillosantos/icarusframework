/**
 * @author J. Santos <jamillo@gmail.com>
 * @date March 12, 2016
 **/

#ifndef ICARUSFRAMEWORK_ROUTES_TYPECONVERSION_H
#define ICARUSFRAMEWORK_ROUTES_TYPECONVERSION_H

namespace icarus
{
/**
 * Based on SOCI ideas for conversion.
 */
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
