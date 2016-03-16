/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date September 09, 2015
 */

#ifndef ICARUSFRAMEWORK_ROUTES_COMPILER_H
#define ICARUSFRAMEWORK_ROUTES_COMPILER_H

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
	void compile(std::string from, std::ostream &to);
};
}
}

#endif // ICARUSFRAMEWORK_ROUTES_COMPILER_H
