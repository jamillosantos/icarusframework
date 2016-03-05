/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date September 09, 2015
 */

#ifndef ICARUSFRAMEWORK_ROUTES_COMPILER_HPP
#define ICARUSFRAMEWORK_ROUTES_COMPILER_HPP

#include <sstream>

#include <boost/filesystem.hpp>

#include <icarus/routes/parser.h>
#include <icarus/routes/routeswriter.h>

namespace icarus
{
namespace routes
{
class compiler
{
public:
	void compile(std::string from, std::string to);
};
}
}

#endif // ICARUSFRAMEWORK_ROUTES_COMPILER_HPP