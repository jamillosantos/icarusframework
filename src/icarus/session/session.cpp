/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 20, 2016
 **/

#include <icarus/session/session.h>
#include <icarus/digests.h>

icarus::session::manager::manager()
	: _maxAge(600), _running(false)
{ }

icarus::session::manager::~manager()
{
	if (this->_running)
		this->stop();
}

unsigned int icarus::session::manager::max_age()
{
	return this->_maxAge;
}

icarus::session::manager &icarus::session::manager::max_age(unsigned int maxAge)
{
	this->_maxAge = maxAge;
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

icarus::session::session_id_t icarus::session::manager::generate_id()
{
	std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
	return icarus::digests::sha256(
		std::to_string(std::rand())
		+ std::to_string(std::chrono::duration_cast<std::chrono::nanoseconds>(now.time_since_epoch()).count())
	);
}

icarus::session::session::session(icarus::session::session &session)
	: _manager(session._manager), _client(session._client), _id(session._id)
{ }

icarus::session::session::session(icarus::session::session &&session)
	: _manager(session._manager), _client(session._client), _id(session._id)
{ }

icarus::session::session::session(icarus::session::manager &manager, icarus::http::client_context &client)
	: _manager(manager), _client(client)
{
	boost::optional<icarus::http::cookie_value&> session_id = this->_client.request().cookies()["session_id"];
	if (session_id)
	{
		this->_id = session_id->value();
	}
	else
	{
		this->_id = this->_manager.generate_id();
		client.response().cookies().set("session_id", this->id());
	}
}

icarus::session::session::~session()
{ }

const icarus::session::session_id_t icarus::session::session::id()
{
	return this->_id;
}

void icarus::session::session::renew()
{ }
