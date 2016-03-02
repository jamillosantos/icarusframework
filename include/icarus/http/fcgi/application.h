/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 21, 2016
 **/

#ifndef ICARUSFRAMEWORK_HTTP_FCGI_APPLICATION_HPP
#define ICARUSFRAMEWORK_HTTP_FCGI_APPLICATION_HPP

#include <icarus/application.h>

#include <fcgiapp.h>

#include <icarus/http/fcgi/client.h>
#include <icarus/http/fcgi/response.h>
#include <icarus/http/fcgi/request.h>

#include <thread>

namespace icarus
{
namespace http
{
namespace fcgi
{
class application
	: public icarus::application
{
protected:
	virtual void init() override;

	virtual http::client_context *accept() override;
};

}
}
}

#endif //ICARUSFRAMEWORK_HTTP_FCGI_APPLICATION_HPP
