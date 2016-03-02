/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 20, 2016
 **/

#include <icarus/session/session.h>

icarus::session::session::session(icarus::session::session_id_t id)
	: _id(id)
{
	this->renew();
}

const icarus::session::session_id_t icarus::session::session::id()
{
	return this->_id;
}

const std::chrono::system_clock::time_point &icarus::session::session::last_used()
{
	return this->_last_used;
}

void icarus::session::session::renew()
{
	this->_last_used = std::chrono::system_clock::now();
}

std::string icarus::session::session::get(const std::string &name)
{ }

std::string icarus::session::session::set(const std::string &name, const std::string &value)
{ }

std::string icarus::session::session::as_string(const std::string &name)
{
	return this->get(name);
}

icarus::session::session &icarus::session::session::as_string(const std::string &name, const std::string &value)
{
	this->set(name, value);
	return *this;
}

template<typename T>
void icarus::session::session::set(const std::string &name, const T &value)
{
	this->set(name, std::to_string(value));
}

int8_t icarus::session::session::as_int8(const std::string &name)
{
	return boost::lexical_cast<int8_t>(this->get(name));
}

uint8_t icarus::session::session::as_uint8(const std::string &name)
{
	return boost::lexical_cast<uint8_t>(this->get(name));
}

short icarus::session::session::as_short(const std::string &name)
{
	return boost::lexical_cast<short>(this->get(name));
}

unsigned short icarus::session::session::as_ushort(const std::string &name)
{
	return boost::lexical_cast<unsigned short>(this->get(name));
}

int icarus::session::session::as_int(const std::string &name)
{
	return boost::lexical_cast<int>(this->get(name));
}

unsigned int icarus::session::session::as_uint(const std::string &name)
{
	return boost::lexical_cast<unsigned int>(this->get(name));
}

long icarus::session::session::as_long(const std::string &name)
{
	return boost::lexical_cast<long>(this->get(name));
}

unsigned long icarus::session::session::as_ulong(const std::string &name)
{
	return boost::lexical_cast<unsigned long>(this->get(name));
}

long long icarus::session::session::as_long_long(const std::string &name)
{
	return boost::lexical_cast<long long>(this->get(name));
}

unsigned long long icarus::session::session::as_ulong_long(const std::string &name)
{
	return boost::lexical_cast<unsigned long long>(this->get(name));
}

float icarus::session::session::as_float(const std::string &name)
{
	return boost::lexical_cast<float>(this->get(name));
}

double icarus::session::session::as_double(const std::string &name)
{
	return boost::lexical_cast<double>(this->get(name));
}

long double icarus::session::session::as_long_double(const std::string &name)
{
	return boost::lexical_cast<long double>(this->get(name));
}
