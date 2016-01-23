/**
 * @author J. Santos <jamillo@gmail.com>
 * @date November 11, 2015
 **/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Routes1

#include <boost/test/included/unit_test.hpp>

#include "../../../icarus/routes/data.hpp"

BOOST_AUTO_TEST_CASE(CallMethodPath1)
{
	icarus::routes::CallMethod method;
	method.path("package::for::methodName");

	BOOST_REQUIRE_EQUAL(method.path().size(), 2);
	BOOST_CHECK_EQUAL(method.path()[0], "package");
	BOOST_CHECK_EQUAL(method.path()[1], "for");
	BOOST_CHECK_EQUAL(method.name(), "methodName");
}

BOOST_AUTO_TEST_CASE(CallMethodPath2)
{
	icarus::routes::CallMethod method;
	method.path("p::f::another::m");

	BOOST_REQUIRE_EQUAL(method.path().size(), 3);
	BOOST_CHECK_EQUAL(method.path()[0], "p");
	BOOST_CHECK_EQUAL(method.path()[1], "f");
	BOOST_CHECK_EQUAL(method.path()[2], "another");
	BOOST_CHECK_EQUAL(method.name(), "m");
}
