/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 24, 2016
 **/

#include <icarus/application.h>

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
{ }

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
			this->_dispatcher.process(client);
			delete client;
		}
	}
	this->cleanup();
}

icarus::dispatcher &icarus::application::dispatcher()
{
	return this->_dispatcher;
}

