/**
 * @author J. Santos <jamillo@gmail.com>
 * @date November 06, 2015
 **/

#ifndef ICARUSFRAMEWORK_EXCEPTIONS_HPP
#define ICARUSFRAMEWORK_EXCEPTIONS_HPP

#include <string>
#include <boost/locale.hpp>

namespace icarus
{
namespace exceptions
{
	namespace bl = boost::locale;

class Exception
	: public std::exception
{
private:
	std::string _message;
	std::exception *_cause;
public:
	Exception(const std::string &message, std::exception *cause)
		: _message(message), _cause(cause)
	{ }

	Exception(const std::string &message)
		: _message(message), _cause(nullptr)
	{ }

	virtual ~Exception()
	{ }


	virtual const char *what() const _GLIBCXX_USE_NOEXCEPT
	{
		return this->_message.c_str();
	}

	std::string message()
	{
		return this->_message;
	}

	std::exception *cause()
	{
		return this->_cause;
	}
};

class FileNotFound
	: Exception
{
public:
	FileNotFound(std::string fileName, std::exception *cause)
		: Exception((bl::format(bl::translate("File '{1}' was not found.")) % fileName).str(), cause)
	{ }

	FileNotFound(std::string fileName)
		: FileNotFound(fileName, nullptr)
	{ }
};

class OpenFile
	: public Exception
{

public:
	OpenFile(const std::string &message, std::exception *cause)
		: Exception(message, cause)
	{ }

	OpenFile(const std::string &message)
		: Exception(message)
	{ }
};

class PrematureEOF
		: public Exception
{
public:
	PrematureEOF()
			: Exception(bl::translate("Premature end of the file."))
	{ }
};
}
}


#endif //ICARUSFRAMEWORK_EXCEPTIONS_HPP
