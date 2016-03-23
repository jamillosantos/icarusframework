/**
 * @author J. Santos <jamillo@gmail.com>
 * @date November 11, 2015
 **/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE IcarusRoutesData

#include <boost/test/unit_test.hpp>

#include <icarus/routes/data.h>

BOOST_AUTO_TEST_CASE(CallMethodPath1)
{
	icarus::routes::call_method method;
	method.path("package::for::methodName");

	BOOST_REQUIRE_EQUAL(method.path().size(), 2);
	BOOST_CHECK_EQUAL(method.path()[0], "package");
	BOOST_CHECK_EQUAL(method.path()[1], "for");
	BOOST_CHECK_EQUAL(method.name(), "methodName");
}

BOOST_AUTO_TEST_CASE(CallMethodPath2)
{
	icarus::routes::call_method method;
	method.path("p::f::another::m");

	BOOST_REQUIRE_EQUAL(method.path().size(), 3);
	BOOST_CHECK_EQUAL(method.path()[0], "p");
	BOOST_CHECK_EQUAL(method.path()[1], "f");
	BOOST_CHECK_EQUAL(method.path()[2], "another");
	BOOST_CHECK_EQUAL(method.name(), "m");
}

BOOST_AUTO_TEST_CASE(RouteStaticInitialization)
{
	icarus::routes::route route(0, "GET", {
		{"test1", ""},
		{"", "test2"},
		{"test3_name", "test3_regex"}
	});

	BOOST_CHECK_EQUAL(route.line(), 0);
	BOOST_REQUIRE_EQUAL(route.uri().size(), 3);
	BOOST_CHECK_EQUAL(route.uri().tokens()[0].name(), "test1");
	BOOST_CHECK_EQUAL(route.uri().tokens()[0].regex(), "");
	BOOST_CHECK_EQUAL(route.uri().tokens()[1].name(), "");
	BOOST_CHECK_EQUAL(route.uri().tokens()[1].regex(), "test2");
	BOOST_CHECK_EQUAL(route.uri().tokens()[2].name(), "test3_name");
	BOOST_CHECK_EQUAL(route.uri().tokens()[2].regex(), "test3_regex");
}
