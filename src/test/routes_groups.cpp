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

	boost::filesystem::path routePath();
	icarus::routes::Parser parser((resourceDir / "routes").string());
	icarus::routes::Document parserData("routes_groups");
	parser.compile((resourceDir / "routes" / "routes_groups").string(), parserData);

	{
		icarus::routes::Group &group = *dynamic_cast<icarus::routes::Group *>(parserData.pieces()[0].get());
		BOOST_REQUIRE_EQUAL(group.uri().tokens().size(), 1);
		BOOST_CHECK_EQUAL(group.uri().tokens()[0].regex(), "/testing/");
		BOOST_CHECK_EQUAL(group.uri().tokens()[0].name(), "");

		BOOST_REQUIRE_EQUAL(group.pieces().size(), 2);
		{
			icarus::routes::Route &line = *dynamic_cast<icarus::routes::Route *>(group.pieces()[0].get());
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

			BOOST_CHECK_EQUAL(line.callMethod().path(), "controllers::Index::default3");
			BOOST_REQUIRE_EQUAL(line.callMethod().params().size(), 2);
			BOOST_CHECK_EQUAL(line.callMethod().params()[0].name(), "count");
			BOOST_CHECK_EQUAL(line.callMethod().params()[0].type(), "unsigned int");
			BOOST_CHECK_EQUAL(line.callMethod().params()[1].name(), "id");
			BOOST_CHECK_EQUAL(line.callMethod().params()[1].type(), "std::string");
		}

		{
			icarus::routes::Route &line = *dynamic_cast<icarus::routes::Route *>(group.pieces()[1].get());
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

			BOOST_CHECK_EQUAL(line.callMethod().path(), "controllers::Index::default4");
			BOOST_REQUIRE_EQUAL(line.callMethod().params().size(), 2);
			BOOST_CHECK_EQUAL(line.callMethod().params()[0].name(), "count");
			BOOST_CHECK_EQUAL(line.callMethod().params()[0].type(), "uint8_t");
			BOOST_CHECK_EQUAL(line.callMethod().params()[1].name(), "id");
			BOOST_CHECK_EQUAL(line.callMethod().params()[1].type(), "std::string");
		}
	}

	{
		icarus::routes::Group &group = *dynamic_cast<icarus::routes::Group *>(parserData.pieces()[1].get());
		BOOST_REQUIRE_EQUAL(group.uri().tokens().size(), 3);
		BOOST_CHECK_EQUAL(group.uri().tokens()[0].regex(), "/u/");
		BOOST_CHECK_EQUAL(group.uri().tokens()[0].name(), "");
		BOOST_CHECK_EQUAL(group.uri().tokens()[1].regex(), "");
		BOOST_CHECK_EQUAL(group.uri().tokens()[1].name(), "user");
		BOOST_CHECK_EQUAL(group.uri().tokens()[2].regex(), "/");
		BOOST_CHECK_EQUAL(group.uri().tokens()[2].name(), "");

		BOOST_REQUIRE_EQUAL(group.pieces().size(), 1);
		{
			icarus::routes::Route &line = *dynamic_cast<icarus::routes::Route *>(group.pieces()[0].get());
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

			BOOST_CHECK_EQUAL(line.callMethod().path(), "controllers::Index::default5");
			BOOST_REQUIRE_EQUAL(line.callMethod().params().size(), 2);
			BOOST_CHECK_EQUAL(line.callMethod().params()[0].name(), "count");
			BOOST_CHECK_EQUAL(line.callMethod().params()[0].type(), "unsigned int");
			BOOST_CHECK_EQUAL(line.callMethod().params()[1].name(), "id");
			BOOST_CHECK_EQUAL(line.callMethod().params()[1].type(), "std::string");
		}
	}

}
