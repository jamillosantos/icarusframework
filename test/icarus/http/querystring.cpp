/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 05, 2016
 **/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE http_querystring

#include <boost/test/unit_test.hpp>

#include <icarus/http/querystring.h>

BOOST_AUTO_TEST_CASE(parse)
{
	icarus::http::query_string_values values;
	icarus::http::query_string::parse("name=foo&family=bar", values);

	BOOST_CHECK_EQUAL(values.at("name").value(), "foo");
	BOOST_CHECK_EQUAL(values.at("family").value(), "bar");

	BOOST_CHECK_EQUAL(values["name"].value(), "foo");
	BOOST_CHECK_EQUAL(values["family"].value(), "bar");

	BOOST_CHECK_EQUAL(values["foo"].value(), "");
}

BOOST_AUTO_TEST_CASE(add)
{
	icarus::http::query_string_values values;
	values.add("name", "value1");
	values.add("name", "value2");
	values.add("name", "value3");

	values.add("family", "1value");
	values.add("family", "2value");

	BOOST_REQUIRE_EQUAL(values["name"].size(), 3);
	BOOST_CHECK_EQUAL(values["name"][0], "value1");
	BOOST_CHECK_EQUAL(values["name"][1], "value2");
	BOOST_CHECK_EQUAL(values["name"][2], "value3");

	BOOST_REQUIRE_EQUAL(values["family"].size(), 2);
	BOOST_CHECK_EQUAL(values["family"][0], "1value");
	BOOST_CHECK_EQUAL(values["family"][1], "2value");
}