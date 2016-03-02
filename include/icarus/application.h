/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 24, 2016
 **/

#ifndef ICARUSFRAMEWORK_APPLICATION_HPP
#define ICARUSFRAMEWORK_APPLICATION_HPP

#include <icarus/http/fcgi/client.h>
#include <icarus/config.h>
#include <icarus/http/request.h>
#include <icarus/http/response.h>
#include <icarus/http/client.h>

namespace icarus
{
class application
{
private:
	icarus::config::config _config;
protected:
	volatile bool _running;
public:
	application();

	config::config &config();

	virtual http::client_context* accept() = 0;

	virtual void terminate();

	virtual bool is_running();

	virtual void init();

	virtual void cleanup();

	virtual void run();
};
}

#endif //ICARUSFRAMEWORK_APPLICATION_HPP
