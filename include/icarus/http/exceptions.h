/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 13, 2016
 **/

#ifndef ICARUSFRAMEWORK_HTTP_EXCEPTIONS_H
#define ICARUSFRAMEWORK_HTTP_EXCEPTIONS_H

#include <icarus/exceptions.h>

namespace icarus
{
namespace http
{
class header_sent_exception
	: public icarus::exception
{
public:
	header_sent_exception();

	header_sent_exception(std::exception *cause);
};
}
}

#endif //ICARUSFRAMEWORK_HTTP_EXCEPTIONS_H
