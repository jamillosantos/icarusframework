/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 24, 2016
 **/

#ifndef ICARUSFRAMEWORK_APPLICATION_HPP
#define ICARUSFRAMEWORK_APPLICATION_HPP

#include <icarus/http/fcgi/client.hpp>
#include <icarus/config.hpp>
#include <icarus/http/request.hpp>
#include <icarus/http/response.hpp>
#include <icarus/http/client.hpp>

namespace icarus
{
class Application
{
private:
	icarus::config::Config _config;
protected:
	volatile bool _running;
public:
	Application()
		: _running(false)
	{ }

	config::Config &config()
	{
		return this->_config;
	}

	virtual http::ClientContext* accept() = 0;

	virtual void terminate()
	{
		this->_running = false;
	}

	virtual bool isRunning()
	{
		return this->_running;
	}

	virtual void init()
	{ }

	virtual void cleanup()
	{ }

	virtual void run()
	{
		this->init();
		this->_running = true;
		while (this->_running)
		{
			http::ClientContext *client = this->accept();
			if (client)
			{
				client->process();
				delete client;
			}
		}
		this->cleanup();
	}
};
}

#endif //ICARUSFRAMEWORK_APPLICATION_HPP
