/**
* @author J. Santos <jamillo@gmail.com>
* @date January 13, 2016
**/

#ifndef ICARUSFRAMEWORK_FCGI_REQUEST_HPP
#define ICARUSFRAMEWORK_FCGI_REQUEST_HPP

#include <icarus/http/request.h>
#include <icarus/exceptions.h>
#include <icarus/http/cookies.h>

#include <fcgiapp.h>

namespace icarus
{
namespace http
{
namespace fcgi
{
class request
	: public icarus::http::request
{
public:
	virtual void init(const FCGX_Request &request);

	template<class T>
	request &operator>>(T& t);
};
}
}
}

#endif //ICARUSFRAMEWORK_FCGI_REQUEST_HPP
