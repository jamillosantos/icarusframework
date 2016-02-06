/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 24, 2016
 **/

#ifndef ICARUSFRAMEWORK_APPLICATION_HPP
#define ICARUSFRAMEWORK_APPLICATION_HPP

#include "http/request.hpp"
#include "http/response.hpp"
#include "http/client.hpp"

namespace icarus
{
class Application
{
private:
	volatile bool _running;
protected:
	virtual void init()
	{}

	virtual void accept() = 0;

	virtual void cleanup()
	{}

public:
	virtual void terminate()
	{
		this->_running = false;
	}

	bool isRunning()
	{
		return this->_running;
	}

	virtual void run()
	{
		this->init();
		this->_running = true;
		while (this->_running)
		{
			this->accept();
		}
		this->cleanup();
	}
};
}

#endif //ICARUSFRAMEWORK_APPLICATION_HPP
