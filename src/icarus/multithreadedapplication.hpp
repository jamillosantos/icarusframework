/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 05, 2016
 **/

#ifndef ICARUSFRAMEWORK_MULTITHREADEDAPPLICATION_HPP
#define ICARUSFRAMEWORK_MULTITHREADEDAPPLICATION_HPP

#include "application.hpp"

#include <mutex>

namespace icarus
{
class MultiThreadedApplication
: public Application
{
private:
	std::mutex _acceptMutex;
	Application &_application;
protected:
	virtual void accept() override
	{
		//
	}
public:
	MultiThreadedApplication(Application &application)
		: _application(application)
	{ }
};
}

#endif //ICARUSFRAMEWORK_MULTITHREADEDAPPLICATION_HPP
