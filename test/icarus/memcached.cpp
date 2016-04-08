/**
 * @author J. Santos <jamillo@gmail.com>
 * @date April 05, 2016
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE memcached

#include <boost/test/unit_test.hpp>
#include <icarus/memcached.h>

BOOST_AUTO_TEST_CASE(connect_1)
{
	icarus::config::session_memcached server_config;
	server_config.add("localhost:11211");
	icarus::memcached_server server(server_config);
	server.connect();

	BOOST_CHECK_MESSAGE(server.connected(), "Server not connected.");
}

BOOST_AUTO_TEST_CASE(connect_2)
{
	icarus::config::session_memcached server_config;
	server_config.add("localhost:11211");
	icarus::memcached_server server(server_config);

	server.connect();
	BOOST_CHECK_MESSAGE(server.connected(), "Server not connected.");

	icarus::memcached_session session(server);
	BOOST_CHECK_MESSAGE(session.connected(), "Session not connected.");

}

struct memcached_connection_fixture
{
	std::unique_ptr<icarus::config::session_memcached> server_config;
	std::unique_ptr<icarus::memcached_server> server;

	memcached_connection_fixture()
	{
		this->server_config.reset(new icarus::config::session_memcached());
		this->server_config->add("localhost:11211");
		this->server.reset(new icarus::memcached_server(*this->server_config));

		server->connect();
		BOOST_REQUIRE_MESSAGE(server->connected(), "Server not connected.");
	}
};

BOOST_FIXTURE_TEST_CASE(set_1, memcached_connection_fixture)
{
	icarus::memcached_session session(*server);
	BOOST_CHECK_MESSAGE(session.connected(), "Session not connected.");
	session.set_value("var1", "person 1 1");
	session.set_value("var2", "person 1 2");

	BOOST_CHECK_EQUAL(session.get_value("var1"), "person 1 1");
	BOOST_CHECK_EQUAL(session.get_value("var2"), "person 1 2");
}

BOOST_FIXTURE_TEST_CASE(get_1, memcached_connection_fixture, * boost::unit_test::depends_on("set_1"))
{
	icarus::memcached_session session(*server);
	BOOST_CHECK_MESSAGE(session.connected(), "Session not connected.");

	BOOST_CHECK_EQUAL(session.get_value("var1"), "person 1 1");
	BOOST_CHECK_EQUAL(session.get_value("var2"), "person 1 2");
}
