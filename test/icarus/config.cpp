/**
 * @author J. Santos <jamillo@gmail.com>
 * @date March 14, 2016
 **/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE config

#include <boost/filesystem.hpp>
#include <boost/test/unit_test.hpp>
#include <icarus/config.h>
#include <icarus/exceptions.h>

BOOST_AUTO_TEST_CASE(load_fail_1)
{
	icarus::config::config c;
	boost::filesystem::path p(TEST_RESOURCE_DIR);
	BOOST_REQUIRE_THROW(c.loadFromFile((p / "config_not_to_be_found").string()), icarus::file_not_found);
}

BOOST_AUTO_TEST_CASE(load_fail_2)
{
	icarus::config::config c;
	boost::filesystem::path p(TEST_RESOURCE_DIR);
	BOOST_REQUIRE_THROW(c.loadFromFile(p.string()), icarus::not_a_file);
}

BOOST_AUTO_TEST_CASE(parse_1)
{
	icarus::config::config c;
	boost::filesystem::path p(TEST_RESOURCE_DIR);
	BOOST_REQUIRE_NO_THROW(c.loadFromFile((p / "config").string()));

	BOOST_CHECK_EQUAL(c.threads(), 25);

	BOOST_CHECK_EQUAL(c.databases().size(), 2);

	BOOST_CHECK_MESSAGE(c.databases()["default"], "Cannot find 'default' database configuration.");
	BOOST_CHECK_EQUAL(c.databases()["default"]->driver(), "mysql");
	BOOST_CHECK_EQUAL(c.databases()["default"]->str(), "mysql://db=test password='12345' user=root");
	BOOST_CHECK_EQUAL(c.databases()["default"]->pool_size(), 10);

	BOOST_CHECK_MESSAGE(c.databases()["test"], "Cannot find 'test' database configuration.");
	BOOST_CHECK_EQUAL(c.databases()["test"]->driver(), "mysql");
	BOOST_CHECK_EQUAL(c.databases()["test"]->str(), "mysql://db=test password='12345' user=root");
	BOOST_CHECK_EQUAL(c.databases()["test"]->pool_size(), 13);
}
