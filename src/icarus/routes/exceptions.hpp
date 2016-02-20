/**
 * @author J. Santos <jamillo@gmail.com>
 * @date November 06, 2015
 **/

#ifndef ICARUSFRAMEWORK_ROUTES_EXCEPTIONS_HPP
#define ICARUSFRAMEWORK_ROUTES_EXCEPTIONS_HPP

#include "../exceptions.hpp"

#include <string>
#include <boost/locale.hpp>

namespace icarus
{
namespace exceptions
{
namespace routes
{
class InvalidURI
	: public icarus::exceptions::Exception
{
public:
	InvalidURI(std::string uri)
		: Exception((boost::locale::format(boost::locale::translate("The URI '{1}' is invalid.")) % uri).str(), nullptr)
	{ }
};

class InvalidChar
	: public icarus::exceptions::Exception
{
public:
	InvalidChar(unsigned int line, unsigned int column)
		: Exception((boost::locale::format(boost::locale::translate("Invalid char found at {1}:{2}.")) % line % column).str(), nullptr)
	{ }
};

class IncompleteDeclaration
	: public icarus::exceptions::Exception
{
public:
	IncompleteDeclaration(unsigned int line, unsigned int column)
		: Exception((boost::locale::format(boost::locale::translate("Incomplete declaration at {1}:{2}.")) % line % column).str(), nullptr)
	{ }
};

class InvalidParamName
	: public icarus::exceptions::Exception
{
public:
	InvalidParamName(unsigned int line, std::string paramName)
		: Exception((boost::locale::format(boost::locale::translate("Param {2} was not found. (at {1})")) % line % paramName).str(), nullptr)
	{ }
};
} // routes
} // exceptions
} // icarus

#endif //ICARUSFRAMEWORK_ROUTES_EXCEPTIONS_HPP