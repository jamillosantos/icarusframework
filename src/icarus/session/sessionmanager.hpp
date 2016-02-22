/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 20, 2016
 **/

#ifndef ICARUSFRAMEWORK_SESSIONMANAGER_HPP
#define ICARUSFRAMEWORK_SESSIONMANAGER_HPP

#include "session.hpp"

namespace icarus
{
namespace session
{
class SessionManager
{
private:
	unsigned int _maxAge;
public:
	unsigned int maxAge()
	{
		return this->_maxAge;
	}

	SessionManager maxAge(unsigned int maxAge)
	{
		this->_maxAge = maxAge;
		return *this;
	}

	virtual Session get(unsigned long long clientId) = 0;

	virtual void clear(unsigned long long clientId) = 0;

	void check()
	{
	}
};
}
}

#endif //ICARUSFRAMEWORK_SESSIONMANAGER_HPP
