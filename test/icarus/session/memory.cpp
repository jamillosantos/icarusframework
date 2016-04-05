/**
 * @author J. Santos <jamillo@gmail.com>
 * @date April 01, 2016
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE session_memory

#include <boost/test/unit_test.hpp>

#include <icarus/session/session.h>
#include <icarus/session/memory.h>


using namespace icarus::session;
memory_manager mm;

BOOST_AUTO_TEST_CASE(creating_2)
{
	mm.start();

	session &session = mm.get(1);
	session.set("user.id", 1u);
	session.set_value("name", "foo");
	BOOST_CHECK_EQUAL(session.get<std::string>("name"), "foo");
	BOOST_CHECK_EQUAL(session.get<unsigned int>("name"), 1u);

	mm.erase(session);
}