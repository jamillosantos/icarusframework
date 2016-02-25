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
class header_sent_exception
	: public icarus::exceptions::exception
{
public:
	header_sent_exception()
		: this(nullptr)
	{ }

	header_sent_exception(std::exception *cause)
		: exception("Headers already sent.", cause)
	{ }
};
}
}

#endif //ICARUSFRAMEWORK_HTTP_EXCEPTIONS_HPP
