/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 20, 2016
 **/

#include <icarus/session/manager.h>

icarus::session::session* icarus::session::manager::createSession(icarus::session::session_id_t id)
{
	return new icarus::session::session(id);
}

icarus::session::manager::manager()
	: _maxAge(600), _running(false)
{ }

	unsigned int icarus::session::manager::maxAge()
	{
		return this->_maxAge;
	}

icarus::session::manager &icarus::session::manager::maxAge(unsigned int maxAge)
{
	this->_maxAge = maxAge;
	return *this;
}

void icarus::session::manager::startTrampolin()
{
	while (true)
	{
		this->check();
		sleep(this->maxAge());
	}
}

void icarus::session::manager::start()
{
	this->_running = true;
	this->_thread.reset(new std::thread(&icarus::session::manager::startTrampolin, this));
}

void icarus::session::manager::stop()
{
	this->_running = false;
	if (this->_thread)
	{
		this->_thread->join();
		this->_thread.reset();
	}
}
