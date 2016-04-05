/**
 * @author J. Santos <jamillo@gmail.com>
 * @date April 02, 2016
 */

#include <icarus/memcached.h>

#include <boost/locale/message.hpp>

icarus::memcached_exception::memcached_exception(const std::string &message, std::exception *cause)
	: exception(message, cause)
{ }

icarus::memcached_exception::memcached_exception(const std::string &message)
	: exception(message)
{ }

icarus::memcached_connection_exception::memcached_connection_exception()
	: memcached_exception(bl::translate("Cannot connect to the memcache.").str())
{}

icarus::memcached_not_connected::memcached_not_connected()
	: memcached_exception(bl::translate("Not connected.").str())
{ }

icarus::memcached_key_not_found::memcached_key_not_found(const std::string &key)
	: memcached_exception((bl::format(bl::translate("Key '{1}' not found.")) % key).str())
{ }

memcached_pool_st* icarus::memcached_server::pool_ref()
{
	return this->_pool;
}

icarus::memcached_server::memcached_server(const icarus::config::session_memcached &config)
	: _pool(nullptr), _config(config)
{ }

icarus::memcached_server::~memcached_server()
{
	if (this->connected())
		this->disconnect();
}

bool icarus::memcached_server::connected()
{
	return (this->_pool != nullptr);
}

void icarus::memcached_server::connect()
{
	std::string cstr;
	unsigned int i = 0;
	for (const icarus::config::session_memcached_server &svr : this->_config)
	{
		cstr += "--SERVER=";
		cstr += svr.address();
		++i;
	}
	this->_pool = memcached_pool(cstr.c_str(), cstr.length());
}

void icarus::memcached_server::disconnect()
{
	memcached_pool_destroy(this->_pool);
	this->_pool = nullptr;
}

void icarus::memcached_session::assert_connection()
{
	if (!this->connected())
		throw icarus::memcached_not_connected();
}

icarus::memcached_session::memcached_session()
{ }

icarus::memcached_session::memcached_session(icarus::memcached_server &server)
{
	this->connect(server);
}

icarus::memcached_session::~memcached_session()
{
	if (this->connected())
		this->disconnect();
}

bool icarus::memcached_session::connected() const
{
	return (this->_memcached_st != nullptr);
}

void icarus::memcached_session::connect(icarus::memcached_server &server)
{
	if (this->connected())
		this->disconnect();

	this->_server = server;
	memcached_return_t rc;
	this->_memcached_st = memcached_pool_pop(this->_server->pool_ref(), false, &rc);
	if (!memcached_success(rc))
	{
		throw icarus::memcached_connection_exception();
	}
}

void icarus::memcached_session::connect(icarus::config::session_memcached_server &config)
{
	this->_memcached_st = memcached(config.address().c_str(), config.address().length());
}

void icarus::memcached_session::disconnect()
{
	if (this->_server)
	{
		memcached_pool_push(this->_server->pool_ref(), this->_memcached_st);
		this->_server.reset();
		this->_memcached_st = nullptr;
	}
	else
	{
		memcached_free(this->_memcached_st);
		this->_memcached_st = nullptr;
	}
}

std::string icarus::memcached_session::get_value(const std::string &key)
{
	memcached_return_t rc;
	size_t size;
	char *value = memcached_get(this->_memcached_st, key.c_str(), key.length(), &size, (uint32_t*)0, &rc);
	if (memcached_success(rc))
		return std::string(value, size);

	throw icarus::memcached_key_not_found(key);
}

std::string icarus::memcached_session::get_value(const std::string &group, const std::string &key)
{
	memcached_return_t rc;
	size_t size;
	char *value = memcached_get_by_key(this->_memcached_st, group.c_str(), group.length(), key.c_str(), key.length(), &size, (uint32_t*)0, &rc);
	if (memcached_success(rc))
		return std::string(value, size);

	throw icarus::memcached_key_not_found(key);
}

bool icarus::memcached_session::set_value(const std::string &key, const std::string &value)
{
	this->assert_connection();

	memcached_return_t rc = memcached_set(this->_memcached_st, key.c_str(), key.length(), value.c_str(), value.length(), (time_t)0, (uint32_t)0);
	return memcached_success(rc);
}

bool icarus::memcached_session::set_value(const std::string &key, const std::string &value,
	const time_t duration)
{
	this->assert_connection();

	memcached_return_t rc = memcached_set(this->_memcached_st, key.c_str(), key.length(), value.c_str(), value.length(), duration, (uint32_t)0);
	return memcached_success(rc);
}

bool icarus::memcached_session::set_value(const std::string &group, const std::string &key, const std::string &value)
{
	this->assert_connection();

	memcached_return_t rc = memcached_set_by_key(this->_memcached_st, group.c_str(), group.length(), key.c_str(),
		key.length(), value.c_str(), value.length(), (time_t)0, (uint32_t)0);
	return memcached_success(rc);
}

bool icarus::memcached_session::set_value(const std::string &group, const std::string &key, const std::string &value,
	time_t duration)
{
	this->assert_connection();

	memcached_return_t rc = memcached_set_by_key(this->_memcached_st, group.c_str(), group.length(), key.c_str(),
		key.length(), value.c_str(), value.length(), (time_t)duration, (uint32_t)0);
	return memcached_success(rc);
}
