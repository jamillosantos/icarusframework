/**
 * @author J. Santos <jamillo@gmail.com>
 * @date April 04, 2016
 */

#include <icarus/session/memcached.h>

icarus::session::session_impl* icarus::session::memcached_manager::create(const icarus::session::session_id_t &id)
{
	return new icarus::session::memcached_session(*this, id);
}

icarus::session::memcached_manager::memcached_manager(icarus::config::session_memcached &config)
	: _server(config)
{ }

void icarus::session::memcached_manager::start()
{
	this->_server.connect();
	manager::start();
}

void icarus::session::memcached_manager::stop()
{
	manager::stop();
	this->_server.disconnect();
}

icarus::session::memcached_session::memcached_session(icarus::session::memcached_session &&msession)
	: icarus::session::session_impl(std::move(msession)), _manager(msession._manager), _session(msession._session.release()),
	  _group_key(msession._group_key)
{ }

icarus::session::memcached_session::memcached_session(icarus::session::memcached_manager &manager, const icarus::session::session_id_t &id)
	: icarus::session::session_impl(id), _manager(manager), _session(new icarus::memcached_session(manager._server)), _group_key("session" + this->id())
{ }

std::string icarus::session::memcached_session::get_value(const std::string &key)
{
	try
	{
		return this->_session->get_value(this->_group_key, this->_group_key + "::" + key);
	}
	catch (icarus::memcached_key_not_found&)
	{
		throw icarus::session::session_key_not_found(key);
	}
}

void icarus::session::memcached_session::set_value(const std::string &key, const std::string &value)
{
	this->_session->set_value(this->_group_key, this->_group_key + "::" + key, value, this->_manager.max_age());
}
