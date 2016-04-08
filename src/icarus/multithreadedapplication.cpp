/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 05, 2016
 **/

#include <icarus/multithreadedapplication.h>

icarus::multi_threaded_application::multi_threaded_application(icarus::application &application)
	: _running(false), _application(application)
{ }

icarus::config::config &icarus::multi_threaded_application::config()
{
	return this->_application.config();
}

icarus::session::manager &icarus::multi_threaded_application::session_manager()
{
	return this->_application.session_manager();
}

void icarus::multi_threaded_application::init()
{
	LOG_DEBUG("icarus::multi_threaded_application::init");
	this->_application.init();
}

void icarus::multi_threaded_application::cleanup()
{
	this->_application.cleanup();
}

icarus::http::client_context *icarus::multi_threaded_application::accept()
{
	std::lock_guard<std::mutex> lock(this->_acceptMutex);
	return this->_application.accept();
}

void icarus::multi_threaded_application::runTrampolin()
{
	icarus::dispatcher &dispatcher = this->_application.dispatcher();
	while (this->_running)
	{
		http::client_context *client = this->accept();
		if (client)
		{
			dispatcher.process(this->_application, client);
		}
	}
}

void icarus::multi_threaded_application::run()
{
	this->_running = true;

	this->init();

	for (unsigned int i = 0; i < this->_application.config().threads(); i++)
		this->_threads.emplace_back(new std::thread(&multi_threaded_application::runTrampolin, this));

	for (unsigned int i = 0; i < this->_application.config().threads(); i++)
		this->_threads[i]->join();

	this->cleanup();
}

void icarus::multi_threaded_application::terminate()
{
	this->_running = false;
}

bool icarus::multi_threaded_application::is_running()
{
	return this->_running;
}

icarus::dispatcher &icarus::multi_threaded_application::dispatcher()
{
	return this->_application.dispatcher();
}