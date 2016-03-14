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
class InvalidURI
	: public icarus::exception
{
public:
	InvalidURI(const std::string &uri);
};

class InvalidChar
	: public icarus::exception
{
public:
	InvalidChar(unsigned int line, unsigned int column);
};

class IncompleteDeclaration
	: public icarus::exception
{
public:
	IncompleteDeclaration(unsigned int line, unsigned int column);
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
