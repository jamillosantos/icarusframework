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

class session;

class manager
{
private:
	unsigned int _max_age;
	volatile bool _running;
protected:
	icarus::session::session_id_t generate_id();

	virtual icarus::session::session create(const icarus::session::session_id_t &id) = 0;
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

class session
{
private:
	session_id_t _id;
protected:
	virtual std::string get_value(const std::string &key) = 0;

	virtual void set_value(const std::string &key, const std::string &value) = 0;
public:
	session(const icarus::session::session_id_t &id);
	virtual ~session();

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
	T get(const std::string &name)
	{
		T result;
		icarus::type_conversion<T>::from(this->get_value(name), result);
		return result;
	}
};
}
}

#endif //ICARUSFRAMEWORK_SESSION_H
