/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 20, 2016
 **/

#ifndef ICARUSFRAMEWORK_SESSION_MANAGER_H
#define ICARUSFRAMEWORK_SESSION_MANAGER_H

#include <icarus/session/session.h>

#include <thread>

namespace icarus
{
namespace session
{
class manager
{
private:
	unsigned int _maxAge;
	volatile bool _running;
	std::unique_ptr<std::thread> _thread;
protected:
	virtual void check() = 0;

	virtual session* createSession(session_id_t id);
public:
	manager();

	unsigned int maxAge();

	icarus::session::manager &maxAge(unsigned int maxAge);

	virtual icarus::session::session* get(session_id_t id) = 0;

	virtual void clear(icarus::session::session_id_t id) = 0;

	virtual void startTrampolin();

	virtual void start();

	virtual void stop();
};
}
}

#endif //ICARUSFRAMEWORK_SESSION_MANAGER_H
