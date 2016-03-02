/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 13, 2016
 **/

#include <icarus/http/exceptions.h>

icarus::http::header_sent_exception::header_sent_exception()
	: icarus::http::header_sent_exception::header_sent_exception(nullptr)
{ }

icarus::http::header_sent_exception::header_sent_exception(std::exception *cause)
	: icarus::exception("Headers already sent.", cause)
{ }
