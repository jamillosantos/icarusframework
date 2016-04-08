/**
 * @author J. Santos <jamillo@gmail.com>
 * @date April 02, 2016
 */

#ifndef ICARUS_MEMCACHED_H
#define ICARUS_MEMCACHED_H

#include <libmemcached/memcached.h>
#include <libmemcached/util.h>
#include <icarus/config.h>
#include <icarus/typeconversion.h>
#include "exceptions.h"

namespace icarus
{

class memcached_exception
	: public icarus::exception
{
public:
	memcached_exception(const std::string &message, std::exception *cause);

	memcached_exception(const std::string &message);
};

class memcached_connection_exception
	: public icarus::memcached_exception
{
public:
	memcached_connection_exception();
};

class memcached_not_connected
	: public icarus::memcached_exception
{
public:
	memcached_not_connected();
};

class memcached_key_not_found
	: public icarus::memcached_exception
{
public:
	memcached_key_not_found(const std::string &key);
};

class memcached_session;

class memcached_server
{
	friend class memcached_session;
private:
	memcached_pool_st* _pool;

	icarus::config::session_memcached _config;
protected:
	memcached_pool_st* pool_ref();
public:
	memcached_server(const icarus::config::session_memcached &config);
	~memcached_server();

	bool connected();
	void connect();
	void disconnect();
};

class memcached_session
{
private:
	boost::optional<memcached_server&> _server;

	memcached_st* _memcached_st;

	void assert_connection();
public:
	memcached_session();
	memcached_session(memcached_server &server);
	~memcached_session();

	bool connected() const;

	void connect(memcached_server &server);
	void connect(icarus::config::session_memcached_server &config);

	void disconnect();

	virtual std::string get_value(const std::string &key);
	virtual size_t get_value(const std::string &key, char **buffer);
	virtual std::string get_value(const std::string &group, const std::string &key);
	virtual size_t get_value(const std::string &group, const std::string &key, char **buffer);

	virtual bool set_value(const std::string &key, const std::string &value);
	virtual bool set_value(const std::string &key, const char *value, size_t value_size);

	virtual bool set_value(const std::string &key, const std::string &value, const time_t duration);
	virtual bool set_value(const std::string &key, const char *value, const size_t value_size, const time_t duration);

	virtual bool set_value(const std::string &group, const std::string &key, const std::string &value);
	virtual bool set_value(const std::string &group, const std::string &key, const char *value,
		const size_t value_size);

	virtual bool set_value(const std::string &group, const std::string &key, const std::string &value, const time_t duration);
	virtual bool set_value(const std::string &group, const std::string &key, const char *value, const size_t value_size,
		const time_t duration);
};
}


#endif //ICARUS_MEMCACHED_H
