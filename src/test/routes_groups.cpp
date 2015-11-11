/**
 * @author J. Santos <jamillo@gmail.com>
 * @date November 06, 2015
 **/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Routes1

#include <boost/test/included/unit_test.hpp>

#include "../framework/routes/parser.hpp"

BOOST_AUTO_TEST_CASE(route_group)
{
	boost::filesystem::path resourceDir(TEST_RESOURCE_DIR);
	
	namespace ifr = icarus::framework::routes;

	boost::filesystem::path routePath;
	ifr::Parser parser((resourceDir / "routes").string());
	ifr::Document parserData("routes_groups");
	parser.parse((resourceDir / "routes" / "routes_groups").string(), parserData);

	{
		ifr::Group &group = *dynamic_cast<ifr::Group *>(parserData.pieces()[0].get());
		BOOST_REQUIRE_EQUAL(group.uri().tokens().size(), 1);
		BOOST_CHECK_EQUAL(group.uri().tokens()[0].regex(), "/testing/");
		BOOST_CHECK_EQUAL(group.uri().tokens()[0].name(), "");

		BOOST_REQUIRE_EQUAL(group.pieces().size(), 2);
		{
			ifr::Route &line = *dynamic_cast<ifr::Route *>(group.pieces()[0].get());
			BOOST_CHECK_EQUAL(line.httpMethod(), "POST");

			BOOST_REQUIRE_EQUAL(line.uri().tokens().size(), 5);
			BOOST_CHECK(line.uri().tokens()[0].name().empty());
			BOOST_CHECK_EQUAL(line.uri().tokens()[0].regex(), "/");
			BOOST_CHECK_EQUAL(line.uri().tokens()[1].name(), "count");
			BOOST_CHECK_EQUAL(line.uri().tokens()[1].regex(), "");
			BOOST_CHECK(line.uri().tokens()[2].name().empty());
			BOOST_CHECK_EQUAL(line.uri().tokens()[2].regex(), "/");
			BOOST_CHECK_EQUAL(line.uri().tokens()[3].name(), "id");
			BOOST_CHECK_EQUAL(line.uri().tokens()[3].regex(), "");
			BOOST_CHECK(line.uri().tokens()[4].name().empty());
			BOOST_CHECK_EQUAL(line.uri().tokens()[4].regex(), "/");

			BOOST_REQUIRE_EQUAL(line.callMethod().path().size(), 2);
			BOOST_CHECK_EQUAL(line.callMethod().path()[0], "controllers");
			BOOST_CHECK_EQUAL(line.callMethod().path()[1], "Index");
			BOOST_CHECK_EQUAL(line.callMethod().name(), "default3");

			BOOST_REQUIRE_EQUAL(line.callMethod().params().size(), 2);
			BOOST_CHECK_EQUAL(line.callMethod().params()[0].name(), "count");
			BOOST_CHECK_EQUAL(line.callMethod().params()[0].type(), "unsigned int");
			BOOST_CHECK_EQUAL(line.callMethod().params()[1].name(), "id");
			BOOST_CHECK_EQUAL(line.callMethod().params()[1].type(), "std::string");
		}

		{
			ifr::Route &line = *dynamic_cast<ifr::Route *>(group.pieces()[1].get());
			BOOST_CHECK_EQUAL(line.httpMethod(), "PUT");

			BOOST_REQUIRE_EQUAL(line.uri().tokens().size(), 5);
			BOOST_CHECK(line.uri().tokens()[0].name().empty());
			BOOST_CHECK_EQUAL(line.uri().tokens()[0].regex(), "/testing/");
			BOOST_CHECK_EQUAL(line.uri().tokens()[1].name(), "count");
			BOOST_CHECK_EQUAL(line.uri().tokens()[1].regex(), "");
			BOOST_CHECK(line.uri().tokens()[2].name().empty());
			BOOST_CHECK_EQUAL(line.uri().tokens()[2].regex(), "/");
			BOOST_CHECK_EQUAL(line.uri().tokens()[3].name(), "id");
			BOOST_CHECK_EQUAL(line.uri().tokens()[3].regex(), "");
			BOOST_CHECK(line.uri().tokens()[4].name().empty());
			BOOST_CHECK_EQUAL(line.uri().tokens()[4].regex(), "/test");

			BOOST_REQUIRE_EQUAL(line.callMethod().path().size(), 2);
			BOOST_CHECK_EQUAL(line.callMethod().path()[0], "controllers");
			BOOST_CHECK_EQUAL(line.callMethod().path()[1], "Index");
			BOOST_CHECK_EQUAL(line.callMethod().name(), "default4");

			BOOST_REQUIRE_EQUAL(line.callMethod().params().size(), 2);
			BOOST_CHECK_EQUAL(line.callMethod().params()[0].name(), "count");
			BOOST_CHECK_EQUAL(line.callMethod().params()[0].type(), "uint8_t");
			BOOST_CHECK_EQUAL(line.callMethod().params()[1].name(), "id");
			BOOST_CHECK_EQUAL(line.callMethod().params()[1].type(), "std::string");
		}
	}

	{
		ifr::Group &group = *dynamic_cast<ifr::Group *>(parserData.pieces()[1].get());
		BOOST_REQUIRE_EQUAL(group.uri().tokens().size(), 3);
		BOOST_CHECK_EQUAL(group.uri().tokens()[0].regex(), "/u/");
		BOOST_CHECK_EQUAL(group.uri().tokens()[0].name(), "");
		BOOST_CHECK_EQUAL(group.uri().tokens()[1].regex(), "");
		BOOST_CHECK_EQUAL(group.uri().tokens()[1].name(), "user");
		BOOST_CHECK_EQUAL(group.uri().tokens()[2].regex(), "/");
		BOOST_CHECK_EQUAL(group.uri().tokens()[2].name(), "");

		BOOST_REQUIRE_EQUAL(group.pieces().size(), 1);
		{
			ifr::Route &line = *dynamic_cast<ifr::Route *>(group.pieces()[0].get());
			BOOST_CHECK_EQUAL(line.httpMethod(), "PUT");

			BOOST_REQUIRE_EQUAL(line.uri().tokens().size(), 5);
			BOOST_CHECK(line.uri().tokens()[0].name().empty());
			BOOST_CHECK_EQUAL(line.uri().tokens()[0].regex(), "/testing/");
			BOOST_CHECK_EQUAL(line.uri().tokens()[1].name(), "count");
			BOOST_CHECK_EQUAL(line.uri().tokens()[1].regex(), "");
			BOOST_CHECK(line.uri().tokens()[2].name().empty());
			BOOST_CHECK_EQUAL(line.uri().tokens()[2].regex(), "x");
			BOOST_CHECK_EQUAL(line.uri().tokens()[3].name(), "id");
			BOOST_CHECK_EQUAL(line.uri().tokens()[3].regex(), "");
			BOOST_CHECK(line.uri().tokens()[4].name().empty());
			BOOST_CHECK_EQUAL(line.uri().tokens()[4].regex(), "test");

			BOOST_REQUIRE_EQUAL(line.callMethod().path().size(), 2);
			BOOST_CHECK_EQUAL(line.callMethod().path()[0], "controllers");
			BOOST_CHECK_EQUAL(line.callMethod().path()[1], "Index");
			BOOST_CHECK_EQUAL(line.callMethod().name(), "default5");

			BOOST_REQUIRE_EQUAL(line.callMethod().params().size(), 2);
			BOOST_CHECK_EQUAL(line.callMethod().params()[0].name(), "count");
			BOOST_CHECK_EQUAL(line.callMethod().params()[0].type(), "unsigned int");
			BOOST_CHECK_EQUAL(line.callMethod().params()[1].name(), "id");
			BOOST_CHECK_EQUAL(line.callMethod().params()[1].type(), "std::string");
		}
	}

}
