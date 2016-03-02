/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 23, 2016
 **/

#ifndef ICARUSFRAMEWORK_SESSION_MEMORY_HPP
#define ICARUSFRAMEWORK_SESSION_MEMORY_HPP

#include <icarus/session/manager.h>

#include <map>

namespace icarus
{
namespace session
{
class memory_session_manager
	: public icarus::session::manager
{
private:
	std::map<unsigned long long, std::unique_ptr<session>> _data;
public:
	virtual session* get(unsigned long long id);

	virtual void clear(unsigned long long id);
};
}
}

#endif //ICARUSFRAMEWORK_SESSION_MEMORY_HPP
