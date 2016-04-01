
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Routes1

#include <boost/test/unit_test.hpp>

#include <icarus/routes/parser.h>
#include <icarus/routes/compiler.h>
#include <icarus/routes/fieldtypes.h>

BOOST_AUTO_TEST_CASE(route1_parsing)
{
	namespace ifr = icarus::routes;

	boost::filesystem::path resourceDir(TEST_RESOURCE_DIR);

	boost::filesystem::path routePath;
	ifr::parser parser((resourceDir / "routes").string());
	ifr::document parserData("routes1");
	parser.parse((resourceDir / "routes" / "routes1").string(), parserData);

	{
		ifr::route &line = *dynamic_cast<ifr::route *>(parserData.pieces()[0].get());
		BOOST_CHECK_EQUAL(line.http_method(), "GET");

		BOOST_REQUIRE_EQUAL(line.uri().tokens().size(), 1);
		BOOST_CHECK(line.uri().tokens()[0].name().empty());
		BOOST_CHECK_EQUAL(line.uri().tokens()[0].regex(), "/");

		BOOST_REQUIRE_EQUAL(line.call_method().path().size(), 2);
		BOOST_CHECK_EQUAL(line.call_method().path()[0], "controllers");
		BOOST_CHECK_EQUAL(line.call_method().path()[1], "Index");
		BOOST_CHECK_EQUAL(line.call_method().name(), "default0");
		BOOST_CHECK(line.call_method().params().empty());
	}

	{
		ifr::route &line = *dynamic_cast<ifr::route *>(parserData.pieces()[1].get());
		BOOST_CHECK_EQUAL(line.http_method(), "GET");

		BOOST_REQUIRE_EQUAL(line.uri().tokens().size(), 2);
		BOOST_CHECK(line.uri().tokens()[0].name().empty());
		BOOST_CHECK_EQUAL(line.uri().tokens()[0].regex(), "/match/");
		BOOST_CHECK_EQUAL(line.uri().tokens()[1].name(), "param1");
		BOOST_CHECK_EQUAL(line.uri().tokens()[1].regex(), "[0-9]+");

		BOOST_REQUIRE_EQUAL(line.call_method().path().size(), 2);
		BOOST_CHECK_EQUAL(line.call_method().path()[0], "controllers");
		BOOST_CHECK_EQUAL(line.call_method().path()[1], "Index");
		BOOST_CHECK_EQUAL(line.call_method().name(), "default1");
		BOOST_REQUIRE_EQUAL(line.call_method().params().size(), 1);
		BOOST_CHECK_EQUAL(line.call_method().params()[0].name(), "parameter1");
		BOOST_CHECK_EQUAL(line.call_method().params()[0].type(), "unsigned int");
	}

	{
		ifr::route &line = *dynamic_cast<ifr::route *>(parserData.pieces()[2].get());
		BOOST_CHECK_EQUAL(line.http_method(), "POST");

		BOOST_REQUIRE_EQUAL(line.uri().tokens().size(), 2);
		BOOST_CHECK(line.uri().tokens()[0].name().empty());
		BOOST_CHECK_EQUAL(line.uri().tokens()[0].regex(), "/");
		BOOST_CHECK_EQUAL(line.uri().tokens()[1].name(), "count");
		BOOST_CHECK_EQUAL(line.uri().tokens()[1].regex(), ifr::field_types::types.get("uint8_t"));

		BOOST_REQUIRE_EQUAL(line.call_method().path().size(), 2);
		BOOST_CHECK_EQUAL(line.call_method().path()[0], "controllers");
		BOOST_CHECK_EQUAL(line.call_method().path()[1], "Index");
		BOOST_CHECK_EQUAL(line.call_method().name(), "default2");
		BOOST_REQUIRE_EQUAL(line.call_method().params().size(), 1);
		BOOST_CHECK_EQUAL(line.call_method().params()[0].name(), "count");
		BOOST_CHECK_EQUAL(line.call_method().params()[0].type(), "uint8_t");
	}

	{
		ifr::route &line = *dynamic_cast<ifr::route *>(parserData.pieces()[3].get());
		BOOST_CHECK_EQUAL(line.http_method(), "POST");

		BOOST_REQUIRE_EQUAL(line.uri().tokens().size(), 4);
		BOOST_CHECK(line.uri().tokens()[0].name().empty());
		BOOST_CHECK_EQUAL(line.uri().tokens()[0].regex(), "/");
		BOOST_CHECK_EQUAL(line.uri().tokens()[1].name(), "count");
		BOOST_CHECK_EQUAL(line.uri().tokens()[1].regex(), ifr::field_types::types.get("unsigned int"));
		BOOST_CHECK(line.uri().tokens()[2].name().empty());
		BOOST_CHECK_EQUAL(line.uri().tokens()[2].regex(), "/");
		BOOST_CHECK_EQUAL(line.uri().tokens()[3].name(), "id");
		BOOST_CHECK_EQUAL(line.uri().tokens()[3].regex(), "");

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
		ifr::route &line = *dynamic_cast<ifr::route *>(parserData.pieces()[4].get());
		BOOST_CHECK_EQUAL(line.http_method(), "PUT");

		BOOST_REQUIRE_EQUAL(line.uri().tokens().size(), 5);
		BOOST_CHECK(line.uri().tokens()[0].name().empty());
		BOOST_CHECK_EQUAL(line.uri().tokens()[0].regex(), "/testing/");
		BOOST_CHECK_EQUAL(line.uri().tokens()[1].name(), "count");
		BOOST_CHECK_EQUAL(line.uri().tokens()[1].regex(), ifr::field_types::types.get("uint8_t"));
		BOOST_CHECK(line.uri().tokens()[2].name().empty());
		BOOST_CHECK_EQUAL(line.uri().tokens()[2].regex(), "/");
		BOOST_CHECK_EQUAL(line.uri().tokens()[3].name(), "id");
		BOOST_CHECK_EQUAL(line.uri().tokens()[3].regex(), "");
		BOOST_CHECK(line.uri().tokens()[4].name().empty());
		BOOST_CHECK_EQUAL(line.uri().tokens()[4].regex(), "/test");

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

	{
		ifr::route &line = *dynamic_cast<ifr::route *>(parserData.pieces()[5].get());
		BOOST_CHECK_EQUAL(line.http_method(), "PUT");

		BOOST_REQUIRE_EQUAL(line.uri().tokens().size(), 5);
		BOOST_CHECK(line.uri().tokens()[0].name().empty());
		BOOST_CHECK_EQUAL(line.uri().tokens()[0].regex(), "/testing/");
		BOOST_CHECK_EQUAL(line.uri().tokens()[1].name(), "count");
		BOOST_CHECK_EQUAL(line.uri().tokens()[1].regex(), ifr::field_types::types.get("unsigned int"));
		BOOST_CHECK(line.uri().tokens()[2].name().empty());
		BOOST_CHECK_EQUAL(line.uri().tokens()[2].regex(), "x");
		BOOST_CHECK_EQUAL(line.uri().tokens()[3].name(), "id");
		BOOST_CHECK_EQUAL(line.uri().tokens()[3].regex(), "");
		BOOST_CHECK(line.uri().tokens()[4].name().empty());
		BOOST_CHECK_EQUAL(line.uri().tokens()[4].regex(), "test");

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

BOOST_AUTO_TEST_CASE(route1_match1)
{
	namespace ifr = icarus::routes;

	boost::filesystem::path resourceDir(TEST_RESOURCE_DIR);

	boost::filesystem::path routePath;
	ifr::parser parser((resourceDir / "routes").string());
	ifr::document parserData("routes1");
	parser.parse((resourceDir / "routes" / "routes1").string(), parserData);

	std::vector<std::string> values;
	ifr::piece *piece = parserData.match("GET", "/match/123", values);
	BOOST_REQUIRE_MESSAGE(piece != nullptr, "Could not find a route to match the URI.");
	ifr::route *route = dynamic_cast<ifr::route*>(piece);
	BOOST_REQUIRE_MESSAGE(route, "The piece found is not a Route*.");
	BOOST_CHECK_EQUAL(values.size(), 1);
	BOOST_CHECK_EQUAL(values[0], "123");
}

BOOST_AUTO_TEST_CASE(route1_match2)
{
	namespace ifr = icarus::routes;

	boost::filesystem::path resourceDir(TEST_RESOURCE_DIR);

	boost::filesystem::path routePath;
	ifr::parser parser((resourceDir / "routes").string());
	ifr::document parserData("routes1");
	parser.parse((resourceDir / "routes" / "routes1").string(), parserData);

	std::vector<std::string> values;
	ifr::piece *piece = parserData.match("POST", "/testing/1/2/test", values);
	BOOST_REQUIRE_MESSAGE(piece == nullptr, "Should not find a route to match the URI.");
}

BOOST_AUTO_TEST_CASE(route1_match3)
{
	namespace ifr = icarus::routes;

	boost::filesystem::path resourceDir(TEST_RESOURCE_DIR);

	boost::filesystem::path routePath;
	ifr::parser parser((resourceDir / "routes").string());
	ifr::document parserData("routes1");
	parser.parse((resourceDir / "routes" / "routes1").string(), parserData);

	std::vector<std::string> values;
	ifr::piece *piece = parserData.match("PUT", "/testing/1/2/test", values);
	BOOST_REQUIRE_MESSAGE(piece != nullptr, "Could not find a route to match the URI.");
	ifr::route *route = dynamic_cast<ifr::route*>(piece);
	BOOST_REQUIRE_MESSAGE(route, "The piece found is not a Route*.");
	BOOST_REQUIRE_EQUAL(values.size(), 2);
	BOOST_CHECK_EQUAL(values[0], "1");
	BOOST_CHECK_EQUAL(values[1], "2");
}

BOOST_AUTO_TEST_CASE(route1_match4)
{
	namespace ifr = icarus::routes;

	boost::filesystem::path resourceDir(TEST_RESOURCE_DIR);

	boost::filesystem::path routePath;
	ifr::parser parser((resourceDir / "routes").string());
	ifr::document parserData("routes1");
	parser.parse((resourceDir / "routes" / "routes1").string(), parserData);

	std::vector<std::string> values;
	ifr::piece *piece = parserData.match("POST", "/4/foobar", values);
	BOOST_REQUIRE_MESSAGE(piece != nullptr, "Could not find a route to match the URI.");
	ifr::route *route = dynamic_cast<ifr::route*>(piece);
	BOOST_REQUIRE_MESSAGE(route, "The piece found is not a Route*.");
	BOOST_REQUIRE_EQUAL(values.size(), 2);
	BOOST_CHECK_EQUAL(values[0], "4");
	BOOST_CHECK_EQUAL(values[1], "foobar");
}
