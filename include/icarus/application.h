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

class iapplication
{
public:
	virtual icarus::config::config &config() = 0;

	virtual icarus::http::client_context* accept() = 0;

	virtual void terminate() = 0;

	virtual bool is_running() = 0;

	virtual void init() = 0;

	virtual void cleanup() = 0;

	virtual void run() = 0;

	virtual icarus::dispatcher &dispatcher() = 0;

	virtual icarus::session::manager& session_manager() = 0;
};

class application
	: public icarus::iapplication
{
private:
	icarus::config::config _config;
protected:
	volatile bool _running;

	icarus::dispatcher &_dispatcher;
	std::unique_ptr<icarus::session::manager> _session_manager;
public:
	application(icarus::dispatcher &dispatcher);

	config::config &config() override;

	virtual void terminate() override;

	virtual bool is_running() override;

	virtual void init() override;

	virtual void cleanup() override;

	virtual void run() override;

	virtual icarus::dispatcher &dispatcher() override;

	icarus::session::manager& session_manager() override;
};
}

#endif //ICARUSFRAMEWORK_APPLICATION_H
