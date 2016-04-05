/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 24, 2016
 **/

#ifndef ICARUSFRAMEWORK_APPLICATION_H
#define ICARUSFRAMEWORK_APPLICATION_H

#include <icarus/config.h>
#include <icarus/http/client.h>
#include <icarus/dispatcher.h>
#include <icarus/session/session.h>

namespace icarus
{

class no_session_manager_defined
	: public icarus::exception
{
public:
	no_session_manager_defined();
};

class application
{
private:
	icarus::config::config _config;
protected:
	volatile bool _running;

	icarus::dispatcher &_dispatcher;
	std::unique_ptr<icarus::session::manager> _session_manager;
public:
	application(icarus::dispatcher &dispatcher);

	config::config &config();

	virtual http::client_context* accept() = 0;

	virtual void terminate();

	virtual bool is_running();

	virtual void init();

	virtual void cleanup();

	virtual void run();

	virtual icarus::dispatcher &dispatcher();

	icarus::session::manager& session_manager();
};
}

#endif //ICARUSFRAMEWORK_APPLICATION_H
