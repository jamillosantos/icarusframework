/**
 * @author J. Santos <jamillo@gmail.com>
 * @date November 11, 2015
 **/

#include <icarus/routes/to_url.h>

template <typename T>
std::string icarus::to_url(T data)
{
	return std::to_string(data);
}
