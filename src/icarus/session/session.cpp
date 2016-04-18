/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 20, 2016
 **/

#include <icarus/session/session.h>
#include <icarus/digests.h>

icarus::session::session_key_not_found::session_key_not_found(const std::string &key)
	: icarus::exception((boost::locale::format(boost::locale::translate("The session '{1}' was not found.")) % key).str())
{ }

icarus::session::session::session(icarus::session::session_impl *session)
	: _session(session)
{ }

icarus::session::session::session(icarus::session::session &&session)
{
	this->_session.reset(session._session.release());
}

icarus::session::session_impl *icarus::session::session::operator->()
{
	return this->_session.get();
}

icarus::session::session_id_t icarus::session::manager::generate_id()
{
	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
	return icarus::digests::sha256(
		std::to_string(std::rand())
		+ std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count())
	);
}

icarus::session::manager::manager()
	: _max_age(600), _running(false)
{ }

icarus::session::manager::~manager()
{
	if (this->_running)
		this->stop();
}

unsigned int icarus::session::manager::max_age()
{
	return this->_max_age;
}

icarus::session::manager &icarus::session::manager::max_age(unsigned int maxAge)
{
	this->_max_age = maxAge;
	return *this;
}

bool icarus::session::manager::running()
{
	return this->_running;
}

void icarus::session::manager::start()
{
	this->_running = true;
}

void icarus::session::manager::stop()
{
	this->_running = false;
}

icarus::session::session icarus::session::manager::get(icarus::http::client_context &client)
{
	std::string session_id = client.request().cookie("session_id");
	if (session_id.empty())
	{
		icarus::session::session_id_t id = this->generate_id();
		client.response().cookies().set("session_id", id);
		return icarus::session::session(this->create(id));
	}
	else
		return icarus::session::session(this->create(session_id));
}

icarus::session::session_impl::session_impl(const icarus::session::session_id_t &id)
	: _id(id)
{ }

icarus::session::session_impl::~session_impl()
{ }

const icarus::session::session_id_t icarus::session::session_impl::id()
{
	return this->_id;
}

void icarus::session::session_impl::renew()
{ }
