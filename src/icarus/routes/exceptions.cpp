/**
 * @author J. Santos <jamillo@gmail.com>
 * @date November 06, 2015
 **/

#include <icarus/routes/exceptions.h>

icarus::routes::InvalidURI::InvalidURI(const std::string &uri)
	: icarus::exception((boost::locale::format(boost::locale::translate("The URI '{1}' is invalid.")) % uri).str(), nullptr)
{ }

icarus::routes::InvalidChar::InvalidChar(unsigned int line, unsigned int column)
	: icarus::exception((boost::locale::format(boost::locale::translate("Invalid char found at {1}:{2}.")) % line % column).str(), nullptr)
{ }

icarus::routes::IncompleteDeclaration::IncompleteDeclaration(unsigned int line, unsigned int column)
	: icarus::exception((boost::locale::format(boost::locale::translate("Incomplete declaration at {1}:{2}.")) % line % column).str(), nullptr)
{ }

icarus::routes::InvalidParamName::InvalidParamName(unsigned int line, std::string paramName)
	: icarus::exception((boost::locale::format(boost::locale::translate("Param {2} was not found. (at {1})")) % line % paramName).str(), nullptr)
{ }
