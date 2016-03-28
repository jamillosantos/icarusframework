/**
 * @author J. Santos <jamillo@gmail.com>
 * @date November 06, 2015
 **/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Routes1

#include <boost/test/unit_test.hpp>

#include <icarus/routes/parser.h>
#include <icarus/routes/fieldtypes.h>

BOOST_AUTO_TEST_CASE(route_group)
{
	boost::filesystem::path resourceDir(TEST_RESOURCE_DIR);
	
	namespace ifr = icarus::routes;

	boost::filesystem::path routePath;
	ifr::parser parser((resourceDir / "routes").string());
	ifr::document parserData("routes_groups");
	parser.parse((resourceDir / "routes" / "routes_groups").string(), parserData);

	{
		ifr::group &group = *dynamic_cast<ifr::group *>(parserData.pieces()[0].get());
		BOOST_REQUIRE_EQUAL(group.uri().tokens().size(), 1);
		BOOST_CHECK_EQUAL(group.uri().tokens()[0].regex(), "/testing");
		BOOST_CHECK_EQUAL(group.uri().tokens()[0].name(), "");

		BOOST_REQUIRE_EQUAL(group.pieces().size(), 2);
		{
			ifr::route &line = *dynamic_cast<ifr::route *>(group.pieces()[0].get());
			BOOST_CHECK_EQUAL(line.http_method(), "POST");

			BOOST_REQUIRE_EQUAL(line.uri().tokens().size(), 6);
			BOOST_CHECK(line.uri().tokens()[0].name().empty());
			BOOST_CHECK_EQUAL(line.uri().tokens()[0].regex(), "/testing");
			BOOST_CHECK(line.uri().tokens()[1].name().empty());
			BOOST_CHECK_EQUAL(line.uri().tokens()[1].regex(), "/");
			BOOST_CHECK_EQUAL(line.uri().tokens()[2].name(), "count");
			BOOST_CHECK_EQUAL(line.uri().tokens()[2].regex(), ifr::field_types::types.get("unsigned int"));
			BOOST_CHECK(line.uri().tokens()[3].name().empty());
			BOOST_CHECK_EQUAL(line.uri().tokens()[3].regex(), "/");
			BOOST_CHECK_EQUAL(line.uri().tokens()[4].name(), "id");
			BOOST_CHECK_EQUAL(line.uri().tokens()[4].regex(), "");
			BOOST_CHECK(line.uri().tokens()[5].name().empty());
			BOOST_CHECK_EQUAL(line.uri().tokens()[5].regex(), "/");

			BOOST_REQUIRE_EQUAL(line.call_method().path().size(), 2);
			BOOST_CHECK_EQUAL(line.call_method().path()[0], "controllers");
			BOOST_CHECK_EQUAL(line.call_method().path()[1], "Index");
			BOOST_CHECK_EQUAL(line.call_method().name(), "default3");

			BOOST_REQUIRE_EQUAL(line.call_method().params().size(), 2);
			BOOST_CHECK_EQUAL(line.call_method().params()[0].name(), "count");
			BOOST_CHECK_EQUAL(line.call_method().params()[0].type(), "unsigned int");
			BOOST_CHECK_EQUAL(line.call_method().params()[1].name(), "id");
			BOOST_CHECK_EQUAL(line.call_method().params()[1].type(), "string");
		}

		{
			ifr::route &line = *dynamic_cast<ifr::route *>(group.pieces()[1].get());
			BOOST_CHECK_EQUAL(line.http_method(), "PUT");

			BOOST_REQUIRE_EQUAL(line.uri().tokens().size(), 6);
			BOOST_CHECK(line.uri().tokens()[0].name().empty());
			BOOST_CHECK_EQUAL(line.uri().tokens()[0].regex(), "/testing");
			BOOST_CHECK(line.uri().tokens()[1].name().empty());
			BOOST_CHECK_EQUAL(line.uri().tokens()[1].regex(), "/testing/");
			BOOST_CHECK_EQUAL(line.uri().tokens()[2].name(), "count");
			BOOST_CHECK_EQUAL(line.uri().tokens()[2].regex(), ifr::field_types::types.get("uint8_t"));
			BOOST_CHECK(line.uri().tokens()[3].name().empty());
			BOOST_CHECK_EQUAL(line.uri().tokens()[3].regex(), "/");
			BOOST_CHECK_EQUAL(line.uri().tokens()[4].name(), "id");
			BOOST_CHECK_EQUAL(line.uri().tokens()[4].regex(), "");
			BOOST_CHECK(line.uri().tokens()[5].name().empty());
			BOOST_CHECK_EQUAL(line.uri().tokens()[5].regex(), "/test");

			BOOST_REQUIRE_EQUAL(line.call_method().path().size(), 2);
			BOOST_CHECK_EQUAL(line.call_method().path()[0], "controllers");
			BOOST_CHECK_EQUAL(line.call_method().path()[1], "Index");
			BOOST_CHECK_EQUAL(line.call_method().name(), "default4");

			BOOST_REQUIRE_EQUAL(line.call_method().params().size(), 2);
			BOOST_CHECK_EQUAL(line.call_method().params()[0].name(), "count");
			BOOST_CHECK_EQUAL(line.call_method().params()[0].type(), "uint8_t");
			BOOST_CHECK_EQUAL(line.call_method().params()[1].name(), "id");
			BOOST_CHECK_EQUAL(line.call_method().params()[1].type(), "string");
		}
	}

	{
		ifr::group &group = *dynamic_cast<ifr::group *>(parserData.pieces()[1].get());
		BOOST_REQUIRE_EQUAL(group.uri().tokens().size(), 2);
		BOOST_CHECK_EQUAL(group.uri().tokens()[0].regex(), "/u/");
		BOOST_CHECK_EQUAL(group.uri().tokens()[0].name(), "");
		BOOST_CHECK_EQUAL(group.uri().tokens()[1].regex(), "");
		BOOST_CHECK_EQUAL(group.uri().tokens()[1].name(), "user");

		BOOST_REQUIRE_EQUAL(group.pieces().size(), 1);
		{
			ifr::route &line = *dynamic_cast<ifr::route *>(group.pieces()[0].get());
			BOOST_CHECK_EQUAL(line.http_method(), "PUT");

			BOOST_REQUIRE_EQUAL(line.uri().tokens().size(), 7);
			BOOST_CHECK(line.uri().tokens()[0].name().empty());
			BOOST_CHECK_EQUAL(line.uri().tokens()[0].regex(), "/u/");
			BOOST_CHECK_EQUAL(line.uri().tokens()[1].name(), "user");
			BOOST_CHECK_EQUAL(line.uri().tokens()[1].regex(), "");
			BOOST_CHECK(line.uri().tokens()[2].name().empty());
			BOOST_CHECK_EQUAL(line.uri().tokens()[2].regex(), "/testing/");
			BOOST_CHECK_EQUAL(line.uri().tokens()[3].name(), "count");
			BOOST_CHECK_EQUAL(line.uri().tokens()[3].regex(), ifr::field_types::types.get("unsigned int"));
			BOOST_CHECK(line.uri().tokens()[4].name().empty());
			BOOST_CHECK_EQUAL(line.uri().tokens()[4].regex(), "x");
			BOOST_CHECK_EQUAL(line.uri().tokens()[5].name(), "id");
			BOOST_CHECK_EQUAL(line.uri().tokens()[5].regex(), "");
			BOOST_CHECK(line.uri().tokens()[6].name().empty());
			BOOST_CHECK_EQUAL(line.uri().tokens()[6].regex(), "test");

			BOOST_REQUIRE_EQUAL(line.call_method().path().size(), 2);
			BOOST_CHECK_EQUAL(line.call_method().path()[0], "controllers");
			BOOST_CHECK_EQUAL(line.call_method().path()[1], "Index");
			BOOST_CHECK_EQUAL(line.call_method().name(), "default5");

			BOOST_REQUIRE_EQUAL(line.call_method().params().size(), 2);
			BOOST_CHECK_EQUAL(line.call_method().params()[0].name(), "count");
			BOOST_CHECK_EQUAL(line.call_method().params()[0].type(), "unsigned int");
			BOOST_CHECK_EQUAL(line.call_method().params()[1].name(), "id");
			BOOST_CHECK_EQUAL(line.call_method().params()[1].type(), "string");
		}
	}
}

BOOST_AUTO_TEST_CASE(route_group_match1)
{
	boost::filesystem::path resourceDir(TEST_RESOURCE_DIR);

	namespace ifr = icarus::routes;

	boost::filesystem::path routePath;
	ifr::parser parser((resourceDir / "routes").string());
	ifr::document parserData("routes_groups");
	parser.parse((resourceDir / "routes" / "routes_groups").string(), parserData);


	icarus::http::value_hash<icarus::http::values_value> values;
	ifr::piece *piece = parserData.match("POST", "/testing/123/abc/", values);
	BOOST_REQUIRE_MESSAGE(piece != nullptr, "Could not find the piece.");
}

BOOST_AUTO_TEST_CASE(route_group_match2)
{
	boost::filesystem::path resourceDir(TEST_RESOURCE_DIR);

	namespace ifr = icarus::routes;

	boost::filesystem::path routePath;
	ifr::parser parser((resourceDir / "routes").string());
	ifr::document parserData("routes_groups");
	parser.parse((resourceDir / "routes" / "routes_groups").string(), parserData);


	icarus::http::value_hash<icarus::http::values_value> values;
	ifr::piece *piece = parserData.match("PUT", "/u/username/testing/123xabctest", values);
	BOOST_REQUIRE_MESSAGE(piece != nullptr, "Could not find the piece.");
	BOOST_CHECK_EQUAL(values.get("user"), "username");
	BOOST_CHECK_EQUAL(values.get("count"), "123");
	BOOST_CHECK_EQUAL(values.get("id"), "abc");
}
