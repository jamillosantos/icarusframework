/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 24, 2016
 **/

#ifndef ICARUSFRAMEWORK_APPLICATION_HPP
#define ICARUSFRAMEWORK_APPLICATION_HPP

#include <icarus/http/fcgi/client.hpp>
#include "http/request.hpp"
#include "http/response.hpp"
#include "http/client.hpp"

namespace icarus
{
class Application
{
protected:
	volatile bool _running;

	virtual void init()
	{ }

	virtual http::ClientContext* accept() = 0;

	virtual void cleanup()
	{ }

public:
	Application()
	{
		this->_running = true;
	}

	virtual void terminate()
	{
		this->_running = false;
	}

	virtual bool isRunning()
	{
		return this->_running;
	}

	virtual void run()
	{
		this->init();
		this->_running = true;
		while (this->_running)
		{
			http::ClientContext *client = this->accept();
			client->process();
			delete client;
		}
		this->cleanup();
	}
};
}

#endif //ICARUSFRAMEWORK_APPLICATION_HPP
