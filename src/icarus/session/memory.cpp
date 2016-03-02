/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 23, 2016
 **/

#include <icarus/session/memory.h>

icarus::session::session *icarus::session::memory_session_manager::get(unsigned long long id)
{
	const std::map<unsigned long long int, std::unique_ptr<icarus::session::session>>::iterator &it = this->_data.find(
		id);
	if (it != this->_data.end())
		return it->second.get();
	else
	{
		icarus::session::session *result = this->createSession(id);
		this->_data.emplace(std::make_pair(id, std::unique_ptr<icarus::session::session>(result)));
		return result;
	}
}

void icarus::session::memory_session_manager::clear(unsigned long long id)
{
	this->_data.erase(id);
}
