/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 20, 2016
 **/

#ifndef ICARUSFRAMEWORK_SESSIONMANAGER_HPP
#define ICARUSFRAMEWORK_SESSIONMANAGER_HPP

#include <thread>

#include "session.hpp"

namespace icarus
{
namespace session
{
class SessionManager
{
private:
	unsigned int _maxAge;
	volatile bool _running;
	std::unique_ptr<std::thread> _thread;
protected:
	virtual void check() = 0;

	virtual Session* createSession(session_id_t id)
	{
		return new Session(id);
	}
public:
	SessionManager()
		: _maxAge(600), _running(false)
	{ }

	unsigned int maxAge()
	{
		return this->_maxAge;
	}

	SessionManager maxAge(unsigned int maxAge)
	{
		this->_maxAge = maxAge;
		return *this;
	}

	virtual Session* get(session_id_t id) = 0;

	virtual void clear(session_id_t id) = 0;

	virtual void startTrampolin()
	{
		while (true)
		{
			this->check();
			sleep(this->maxAge());
		}
	}

	virtual void start()
	{
		this->_running = true;
		this->_thread.reset(new std::thread(startTrampolin, this));
	}

	virtual void stop()
	{
		this->_running = false;
		if (this->_thread)
		{
			this->_thread->join();
			this->_thread.reset();
		}
	}
};
}
}

#endif //ICARUSFRAMEWORK_SESSIONMANAGER_HPP
