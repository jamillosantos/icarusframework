/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 05, 2016
 **/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE HttpHeaders

#include <boost/test/unit_test.hpp>

#include <icarus/http/headers.h>

BOOST_AUTO_TEST_CASE(ValuesList1)
{
	icarus::http::headers values;
	values.emplace("name", "value");

	BOOST_CHECK(values["name"] && !values["name"]->value().empty());
	BOOST_CHECK(!values["name2"]);
	values.emplace("name2", "value2");
	BOOST_CHECK_EQUAL(values["name"]->value(), "value");
	BOOST_CHECK_EQUAL(values["name2"]->value(), "value2");
}