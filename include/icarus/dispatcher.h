/**
 * @author J. Santos <jamillo@gmail.com>
 * @date March 22, 2016
 **/

#ifndef ICARUSFRAMEWORK_DISPATCHER_H
#define ICARUSFRAMEWORK_DISPATCHER_H

#include <icarus/http/client.h>

namespace icarus
{
class dispatcher
{
protected:
	virtual void find_route(http::client_context *client) = 0;
public:
	virtual void process(http::client_context *client);
};
}

#endif //ICARUSFRAMEWORK_DISPATCHER_H
