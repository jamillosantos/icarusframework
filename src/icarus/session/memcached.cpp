/**
 * @author J. Santos <jamillo@gmail.com>
 * @date April 04, 2016
 */

#include <icarus/session/memcached.h>

icarus::session::memcached_manager::memcached_manager(icarus::config::session_memcached &config)
	: _server(config)
{ }

icarus::session::memcached_session::memcached_session(memcached_session &&sss)
	: icarus::session::session(std::move(sss)), _session(sss._session.release()), _group_key(sss._group_key)
{ }

icarus::session::memcached_session::memcached_session(icarus::session::memcached_manager &manager,
	icarus::http::client_context &context)
	: icarus::session::session(manager, context), _session(new icarus::memcached_session(manager._server)), _group_key("session" + this->id())
{ }

std::string icarus::session::memcached_session::get_value(const std::string &key)
{
	return this->_session->get_value(this->_group_key, key);
}

void icarus::session::memcached_session::set_value(const std::string &key, const std::string &value)
{
	this->_session->set_value(this->_group_key, key, value, this->_manager.max_age());
}
