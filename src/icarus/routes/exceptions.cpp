/**
 * @author J. Santos <jamillo@gmail.com>
 * @date November 06, 2015
 **/

#include <icarus/routes/exceptions.h>

icarus::routes::invlid_uri::invlid_uri(const std::string &uri)
	: icarus::exception((boost::locale::format(boost::locale::translate("The URI '{1}' is invalid.")) % uri).str(), nullptr)
{ }

icarus::routes::invalid_Char::invalid_Char(unsigned int line, unsigned int column)
	: icarus::exception((boost::locale::format(boost::locale::translate("Invalid char found at {1}:{2}.")) % line % column).str(), nullptr)
{ }

icarus::routes::incomplete_declaration::incomplete_declaration(unsigned int line, unsigned int column)
	: icarus::exception((boost::locale::format(boost::locale::translate("Incomplete declaration at {1}:{2}.")) % line % column).str(), nullptr)
{ }

icarus::routes::param_not_found::param_not_found(unsigned int line, std::string paramName)
	: icarus::exception((boost::locale::format(boost::locale::translate("Param {2} was not found. (at {1})")) % line % paramName).str(), nullptr)
{ }

icarus::routes::identifier_not_found::identifier_not_found(unsigned int line, std::string identifier)
	: icarus::exception((boost::locale::format(boost::locale::translate("Identifier {2} was not found. (at {1})")) % line % identifier).str(), nullptr)
{ }
