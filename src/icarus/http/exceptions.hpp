/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 13, 2016
 **/

#ifndef ICARUSFRAMEWORK_HTTP_EXCEPTIONS_HPP
#define ICARUSFRAMEWORK_HTTP_EXCEPTIONS_HPP

#include "../exceptions.hpp"

namespace icarus
{
namespace http
{
class HeaderSentException
	: public icarus::exceptions::Exception
{
public:
	HeaderSentException()
		: this(nullptr)
	{ }

	HeaderSentException(std::exception *cause)
		: Exception("Headers already sent.", cause)
	{ }
};
}
}

#endif //ICARUSFRAMEWORK_HTTP_EXCEPTIONS_HPP
