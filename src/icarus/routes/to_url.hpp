/**
 * @author J. Santos <jamillo@gmail.com>
 * @date November 11, 2015
 **/

#ifndef ICARUSFRAMEWORK_TO_URL_HPP
#define ICARUSFRAMEWORK_TO_URL_HPP

#include <string>

namespace icarus
{
template <typename T>
std::string to_url(T data)
{
	return std::to_string(data);
}
}

#endif //ICARUSFRAMEWORK_TO_URL_HPP
