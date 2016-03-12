/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 05, 2016
 **/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE HttpHeaders

#include <boost/test/included/unit_test.hpp>

#include <icarus/http/headers.h>

BOOST_AUTO_TEST_CASE(ValuesList1)
{
	icarus::http::value_list<icarus::http::values_value> values;
	values.add("name", "value");

	BOOST_CHECK(!values.get("name").empty());
	BOOST_CHECK(values.get("name2").empty());
	values.add("name2", "value2");
	BOOST_CHECK_EQUAL(values.get("name"), "value");
	BOOST_CHECK_EQUAL(values.get("name2"), "value2");
}