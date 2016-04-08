/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 20, 2016
 **/

#ifndef ICARUSFRAMEWORK_SESSION_SESSION_H
#define ICARUSFRAMEWORK_SESSION_SESSION_H

#include <string>

#include <boost/lexical_cast.hpp>

#include <icarus/typeconversion.h>
#include <icarus/typeconversion-commons.h>
#include <icarus/http/client.h>

namespace icarus
{
namespace session
{
typedef std::string session_id_t;

class session_impl;

class session_key_not_found
	: public icarus::exception
{
public:
	session_key_not_found(const std::string &key);
};

class session
{
private:
	std::unique_ptr<icarus::session::session_impl> _session;
public:
	session(icarus::session::session_impl* session);
	session(icarus::session::session &&session);

	icarus::session::session_impl * operator->();
};

class manager
{
private:
	unsigned int _max_age;
	volatile bool _running;
protected:
	icarus::session::session_id_t generate_id();

	virtual icarus::session::session_impl* create(const icarus::session::session_id_t &id) = 0;
public:
	manager();
	~manager();

	unsigned int max_age();
	icarus::session::manager &max_age(unsigned int maxAge);

	bool running();
	virtual void start();
	virtual void stop();

	virtual icarus::session::session get(icarus::http::client_context &client);
};

class session_impl
{
private:
	session_id_t _id;
protected:
	virtual std::string get_value(const std::string &key) = 0;

	virtual void set_value(const std::string &key, const std::string &value) = 0;
public:
	session_impl(const icarus::session::session_id_t &id);
	virtual ~session_impl();

	const session_id_t id();

	void renew();

	template <typename T>
	void set(const std::string &name, const T &value)
	{
		std::string v;
		icarus::type_conversion<T>::to(value, v);
		this->set_value(name, v);
	}

	template <typename T>
	void get(const std::string &name, T &out)
	{
		icarus::type_conversion<T>::from(this->get_value(name), out);
	}

	template <typename T>
	T get(const std::string &name, const T &default_value)
	{
		try
		{
			T result;
			icarus::type_conversion<T>::from(this->get_value(name), result);
			return result;
		}
		catch (icarus::session::session_key_not_found&)
		{
			return default_value;
		}
	}
};
}
}

#endif //ICARUSFRAMEWORK_SESSION_H
