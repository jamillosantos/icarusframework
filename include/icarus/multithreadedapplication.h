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
: public iapplication
{
private:
	bool volatile _running;
	std::mutex _acceptMutex;
	application &_application;
	std::vector<std::unique_ptr<std::thread>> _threads;
public:
	multi_threaded_application(icarus::application &application);

	virtual config::config & config() override;

	virtual icarus::session::manager &session_manager() override;

	virtual void init() override;

	virtual void cleanup() override;

	virtual http::client_context *accept() override;

	virtual void runTrampolin();

	virtual void run() override;

	virtual void terminate() override;

	virtual bool is_running() override;

	virtual icarus::dispatcher &dispatcher() override;
};
}

#endif //ICARUSFRAMEWORK_MULTITHREADEDAPPLICATION_H
