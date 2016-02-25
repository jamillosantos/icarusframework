/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 23, 2016
 **/

#ifndef ICARUSFRAMEWORK_MEMORY_HPP
#define ICARUSFRAMEWORK_MEMORY_HPP

#include <map>
#include "sessionmanager.hpp"

namespace icarus
{
namespace session
{
class memory_session_manager
	: public session_manager
{
private:
	std::map<unsigned long long, std::unique_ptr<session>> _data;
public:
	virtual session* get(unsigned long long id) override
	{
		const std::map<unsigned long long int, std::unique_ptr<icarus::session::session>>::iterator &it = this->_data.find(id);
		if (it != this->_data.end())
			return it->second.get();
		else
		{
			session* result = this->createSession(id);
			this->_data.emplace(std::make_pair(id, result));
			return result;
		}
	}

	virtual void clear(unsigned long long id) override
	{
		this->_data.erase(id);
	}
};
}
}

#endif //ICARUSFRAMEWORK_MEMORY_HPP
