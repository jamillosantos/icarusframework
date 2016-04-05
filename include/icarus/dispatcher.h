/**
 * @author J. Santos <jamillo@gmail.com>
 * @date March 22, 2016
 **/

#ifndef ICARUSFRAMEWORK_DISPATCHER_H
#define ICARUSFRAMEWORK_DISPATCHER_H

#include <icarus/http/client.h>

namespace icarus
{

class application;

/** Dispatches client request after accepted by the `application`.
 *
 * This class dispatches client request after accepted by the `application` and after all work done, it should clean up
 * the `client_context` pointer.
 *
 * This extension of this class must be implemented on each client application.
 *
 * @see icarus::application
 */
class dispatcher
{
protected:
	/**
	 * Abstract method to be overrided when implementing a project.
	 */
	virtual void find_route(icarus::application &app, http::client_context *client) = 0;

	/**
	 * Method responsible for cleaning the `client_context` up.
	 */
	virtual void destroy_client(http::client_context *client);
public:
	/**
	 * Process the client request. The default implementation of it is to call the `find_route` method.
	 */
	virtual void process(icarus::application &app, http::client_context *client);
};
}

#endif //ICARUSFRAMEWORK_DISPATCHER_H
