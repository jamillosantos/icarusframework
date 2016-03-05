/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 05, 2016
 **/

#ifndef ICARUSFRAMEWORK_MULTITHREADEDAPPLICATION_H
#define ICARUSFRAMEWORK_MULTITHREADEDAPPLICATION_H

#include <icarus/application.h>

#include <vector>
#include <thread>
#include <mutex>

namespace icarus
{
class multi_threaded_application
: public application
{
private:
	std::mutex _acceptMutex;
	application &_application;
	std::vector<std::unique_ptr<std::thread>> _threads;
public:
	multi_threaded_application(application &application);

	virtual void init() override;

	virtual void cleanup() override;

	virtual http::client_context *accept() override;

	virtual void runTrampolin();

	virtual void run() override;
};
}

#endif //ICARUSFRAMEWORK_MULTITHREADEDAPPLICATION_H
