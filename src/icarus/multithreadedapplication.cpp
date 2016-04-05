/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 05, 2016
 **/

#include <icarus/multithreadedapplication.h>

icarus::multi_threaded_application::multi_threaded_application(icarus::application &application)
	: icarus::application::application(application.dispatcher()), _application(application)
{ }

void icarus::multi_threaded_application::init()
{
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
	LOG_TRACE("Running trampolin");
	while (this->_running)
	{
		http::client_context *client = this->accept();
		if (client)
		{
			this->_dispatcher.process(*this, client);
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
