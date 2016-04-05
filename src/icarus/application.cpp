/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 24, 2016
 **/

#include <icarus/application.h>
#include <icarus/session/memcached.h>

icarus::no_session_manager_defined::no_session_manager_defined()
	: icarus::exception::exception(boost::locale::translate("No session manager defined.").str())
{ }

icarus::application::application(icarus::dispatcher &dispatcher)
	: _running(false), _dispatcher(dispatcher)
{ }

icarus::config::config &icarus::application::config()
{
	return this->_config;
}

void icarus::application::terminate()
{
	this->_running = false;
}

bool icarus::application::is_running()
{
	return this->_running;
}

void icarus::application::init()
{
	if (this->_config.session().memcached())
		this->_session_manager.reset(new icarus::session::memcached_manager(this->config().session().memcached().get()));
	else
		throw icarus::no_session_manager_defined();
}

void icarus::application::cleanup()
{ }

void icarus::application::run()
{
	this->init();
	this->_running = true;
	while (this->_running)
	{
		http::client_context *client = this->accept();
		if (client)
		{
			this->_dispatcher.process(*this, client);
		}
	}
	this->cleanup();
}

icarus::dispatcher &icarus::application::dispatcher()
{
	return this->_dispatcher;
}

icarus::session::manager &icarus::application::session_manager()
{
	return *this->_session_manager;
}
