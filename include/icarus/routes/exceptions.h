/**
 * @author J. Santos <jamillo@gmail.com>
 * @date November 06, 2015
 **/

#ifndef ICARUSFRAMEWORK_ROUTES_EXCEPTIONS_H
#define ICARUSFRAMEWORK_ROUTES_EXCEPTIONS_H

#include <icarus/exceptions.h>

#include <string>
#include <boost/locale.hpp>

namespace icarus
{
namespace routes
{
class invlid_uri
	: public icarus::exception
{
public:
	invlid_uri(const std::string &uri);
};

class invalid_Char
	: public icarus::exception
{
public:
	invalid_Char(unsigned int line, unsigned int column);
};

class incomplete_declaration
	: public icarus::exception
{
public:
	incomplete_declaration(unsigned int line, unsigned int column);
};

class param_not_found
	: public icarus::exception
{
public:
	param_not_found(unsigned int line, std::string paramName);
};
} // routes
} // icarus

#endif //ICARUSFRAMEWORK_ROUTES_EXCEPTIONS_H
