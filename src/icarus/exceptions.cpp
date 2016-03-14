/**
 * @author J. Santos <jamillo@gmail.com>
 * @date November 06, 2015
 **/

#include <icarus/exceptions.h>

namespace bl = boost::locale;

icarus::exception::exception(const std::string &message, std::exception *cause)
	: _message(message), _cause(cause)
{ }

icarus::exception::exception(const std::string &message)
	: _message(message), _cause(nullptr)
{ }

icarus::exception::~exception()
{ }

const char *icarus::exception::what() const _GLIBCXX_USE_NOEXCEPT
{
	return this->_message.c_str();
}

std::string icarus::exception::message()
{
	return this->_message;
}

std::exception *icarus::exception::cause()
{
	return this->_cause;
}

icarus::file_not_found::file_not_found(std::string fileName, std::exception *cause)
	: icarus::exception((bl::format(bl::translate("File '{1}' was not found.")) % fileName).str(), cause)
{ }

icarus::file_not_found::file_not_found(std::string fileName)
	: icarus::file_not_found::file_not_found(fileName, nullptr)
{ }

icarus::open_file::open_file(const std::string &message, std::exception *cause)
	: exception(message, cause)
{ }

icarus::open_file::open_file(const std::string &message)
	: exception(message)
{ }

icarus::premature_eof::premature_eof()
	: icarus::exception(bl::translate("Premature end of the file."))
{ }

icarus::premature_eof::premature_eof(unsigned int line)
	: icarus::exception((bl::format(bl::translate("Premature end of the file at line {1}.")) % line).str())
{ }

