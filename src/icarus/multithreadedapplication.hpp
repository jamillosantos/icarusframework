/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 05, 2016
 **/

#ifndef ICARUSFRAMEWORK_MULTITHREADEDAPPLICATION_HPP
#define ICARUSFRAMEWORK_MULTITHREADEDAPPLICATION_HPP

#include "application.hpp"

#include <vector>
#include <thread>
#include <mutex>

namespace icarus
{
class MultiThreadedApplication
: public Application
{
private:
	std::mutex _acceptMutex;
	Application &_application;
	std::vector<std::unique_ptr<std::thread>> _threads;
public:
	MultiThreadedApplication(Application &application)
		: _application(application)
	{ }

	virtual void init() override
	{
		this->_application.init();
	}

	virtual void cleanup() override
	{
		this->_application.cleanup();
	}

	virtual http::ClientContext *accept() override
	{
		std::lock_guard<std::mutex> lock(this->_acceptMutex);
		return this->_application.accept();
	}

	virtual void runTrampolin()
	{
		LOG_TRACE("Running trampolin");
		while (this->_running)
		{
			http::ClientContext *client = this->accept();
			if (client)
			{
				client->process();
				delete client;
			}
		}
	}

	virtual void run() override
	{
		this->_running = true;

		this->init();

		for (unsigned int i = 0; i < this->_application.config().threads(); i++)
			this->_threads.emplace_back(new std::thread(&MultiThreadedApplication::runTrampolin, this));

		for (unsigned int i = 0; i < this->_application.config().threads(); i++)
			this->_threads[i]->join();

		this->cleanup();
	}
};
}

#endif //ICARUSFRAMEWORK_MULTITHREADEDAPPLICATION_HPP
