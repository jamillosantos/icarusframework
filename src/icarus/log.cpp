/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 21, 2016
 **/

#include <icarus/log.h>

const std::string icarus::log::fname(const std::string &name)
{
	return name.substr(SOURCE_DIRECTORY_LENGTH);
}
