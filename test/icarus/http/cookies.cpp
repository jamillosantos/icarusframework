/**
 * @author J. Santos <jamillo@gmail.com>
 * @date April 08, 2016
 */

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE http_cookies

#include <boost/test/unit_test.hpp>
#include <icarus/http/cookies.h>

BOOST_AUTO_TEST_CASE(set_and_get)
{
	icarus::http::cookies values;

	values.emplace("cookie1", "value1");
	values.emplace("cookie2", "value2");
	BOOST_CHECK_MESSAGE(!values["cookie0"], "'cookie0' should not be found.");

	BOOST_REQUIRE_MESSAGE(values["cookie1"], "'cookie1' shoud be found.");
	BOOST_CHECK_EQUAL(values["cookie1"]->value(), "value1");

	BOOST_REQUIRE_MESSAGE(values["cookie2"], "'cookie2' shoud be found.");
	BOOST_CHECK_EQUAL(values["cookie2"]->value(), "value2");
}