#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Routes1

#include <boost/test/included/unit_test.hpp>

#include "../../../icarus/routes/parser.hpp"
#include "../../../icarus/routes/compiler.hpp"

BOOST_AUTO_TEST_CASE(route1_parsing)
{
	namespace ifr = icarus::routes;

	boost::filesystem::path resourceDir(TEST_RESOURCE_DIR);

	boost::filesystem::path routePath;
	ifr::Parser parser((resourceDir / "routes").string());
	ifr::Document parserData("routes1");
	parser.parse((resourceDir / "routes" / "routes1").string(), parserData);

	{
		ifr::Route &line = *dynamic_cast<ifr::Route *>(parserData.pieces()[0].get());
		BOOST_CHECK_EQUAL(line.httpMethod(), "GET");

		BOOST_REQUIRE_EQUAL(line.uri().tokens().size(), 1);
		BOOST_CHECK(line.uri().tokens()[0].name().empty());
		BOOST_CHECK_EQUAL(line.uri().tokens()[0].regex(), "/");

		BOOST_REQUIRE_EQUAL(line.callMethod().path().size(), 2);
		BOOST_CHECK_EQUAL(line.callMethod().path()[0], "controllers");
		BOOST_CHECK_EQUAL(line.callMethod().path()[1], "Index");
		BOOST_CHECK_EQUAL(line.callMethod().name(), "default0");
		BOOST_CHECK(line.callMethod().params().empty());
	}

	{
		ifr::Route &line = *dynamic_cast<ifr::Route *>(parserData.pieces()[1].get());
		BOOST_CHECK_EQUAL(line.httpMethod(), "GET");

		BOOST_REQUIRE_EQUAL(line.uri().tokens().size(), 2);
		BOOST_CHECK(line.uri().tokens()[0].name().empty());
		BOOST_CHECK_EQUAL(line.uri().tokens()[0].regex(), "/match/");
		BOOST_CHECK_EQUAL(line.uri().tokens()[1].name(), "param1");
		BOOST_CHECK_EQUAL(line.uri().tokens()[1].regex(), "[0-9]+");

		BOOST_REQUIRE_EQUAL(line.callMethod().path().size(), 2);
		BOOST_CHECK_EQUAL(line.callMethod().path()[0], "controllers");
		BOOST_CHECK_EQUAL(line.callMethod().path()[1], "Index");
		BOOST_CHECK_EQUAL(line.callMethod().name(), "default1");
		BOOST_REQUIRE_EQUAL(line.callMethod().params().size(), 1);
		BOOST_CHECK_EQUAL(line.callMethod().params()[0].name(), "parameter1");
		BOOST_CHECK_EQUAL(line.callMethod().params()[0].type(), "unsigned int");
	}

	{
		ifr::Route &line = *dynamic_cast<ifr::Route *>(parserData.pieces()[2].get());
		BOOST_CHECK_EQUAL(line.httpMethod(), "POST");

		BOOST_REQUIRE_EQUAL(line.uri().tokens().size(), 2);
		BOOST_CHECK(line.uri().tokens()[0].name().empty());
		BOOST_CHECK_EQUAL(line.uri().tokens()[0].regex(), "/");
		BOOST_CHECK_EQUAL(line.uri().tokens()[1].name(), "count");
		BOOST_CHECK_EQUAL(line.uri().tokens()[1].regex(), ifr::fieldTypes.get("uint8_t"));

		BOOST_REQUIRE_EQUAL(line.callMethod().path().size(), 2);
		BOOST_CHECK_EQUAL(line.callMethod().path()[0], "controllers");
		BOOST_CHECK_EQUAL(line.callMethod().path()[1], "Index");
		BOOST_CHECK_EQUAL(line.callMethod().name(), "default2");
		BOOST_REQUIRE_EQUAL(line.callMethod().params().size(), 1);
		BOOST_CHECK_EQUAL(line.callMethod().params()[0].name(), "count");
		BOOST_CHECK_EQUAL(line.callMethod().params()[0].type(), "uint8_t");
	}

	{
		ifr::Route &line = *dynamic_cast<ifr::Route *>(parserData.pieces()[3].get());
		BOOST_CHECK_EQUAL(line.httpMethod(), "POST");

		BOOST_REQUIRE_EQUAL(line.uri().tokens().size(), 4);
		BOOST_CHECK(line.uri().tokens()[0].name().empty());
		BOOST_CHECK_EQUAL(line.uri().tokens()[0].regex(), "/");
		BOOST_CHECK_EQUAL(line.uri().tokens()[1].name(), "count");
		BOOST_CHECK_EQUAL(line.uri().tokens()[1].regex(), ifr::fieldTypes.get("unsigned int"));
		BOOST_CHECK(line.uri().tokens()[2].name().empty());
		BOOST_CHECK_EQUAL(line.uri().tokens()[2].regex(), "/");
		BOOST_CHECK_EQUAL(line.uri().tokens()[3].name(), "id");
		BOOST_CHECK_EQUAL(line.uri().tokens()[3].regex(), "");

		BOOST_REQUIRE_EQUAL(line.callMethod().path().size(), 2);
		BOOST_CHECK_EQUAL(line.callMethod().path()[0], "controllers");
		BOOST_CHECK_EQUAL(line.callMethod().path()[1], "Index");
		BOOST_CHECK_EQUAL(line.callMethod().name(), "default3");
		BOOST_REQUIRE_EQUAL(line.callMethod().params().size(), 2);
		BOOST_CHECK_EQUAL(line.callMethod().params()[0].name(), "count");
		BOOST_CHECK_EQUAL(line.callMethod().params()[0].type(), "unsigned int");
		BOOST_CHECK_EQUAL(line.callMethod().params()[1].name(), "id");
		BOOST_CHECK_EQUAL(line.callMethod().params()[1].type(), "string");
	}

	{
		ifr::Route &line = *dynamic_cast<ifr::Route *>(parserData.pieces()[4].get());
		BOOST_CHECK_EQUAL(line.httpMethod(), "PUT");

		BOOST_REQUIRE_EQUAL(line.uri().tokens().size(), 5);
		BOOST_CHECK(line.uri().tokens()[0].name().empty());
		BOOST_CHECK_EQUAL(line.uri().tokens()[0].regex(), "/testing/");
		BOOST_CHECK_EQUAL(line.uri().tokens()[1].name(), "count");
		BOOST_CHECK_EQUAL(line.uri().tokens()[1].regex(), ifr::fieldTypes.get("uint8_t"));
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
		BOOST_CHECK_EQUAL(line.callMethod().params()[1].type(), "string");
	}

	{
		ifr::Route &line = *dynamic_cast<ifr::Route *>(parserData.pieces()[5].get());
		BOOST_CHECK_EQUAL(line.httpMethod(), "PUT");

		BOOST_REQUIRE_EQUAL(line.uri().tokens().size(), 5);
		BOOST_CHECK(line.uri().tokens()[0].name().empty());
		BOOST_CHECK_EQUAL(line.uri().tokens()[0].regex(), "/testing/");
		BOOST_CHECK_EQUAL(line.uri().tokens()[1].name(), "count");
		BOOST_CHECK_EQUAL(line.uri().tokens()[1].regex(), ifr::fieldTypes.get("unsigned int"));
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
		BOOST_CHECK_EQUAL(line.callMethod().params()[1].type(), "string");
	}
}

BOOST_AUTO_TEST_CASE(route1_match1)
{
	namespace ifr = icarus::routes;

	boost::filesystem::path resourceDir(TEST_RESOURCE_DIR);

	boost::filesystem::path routePath;
	ifr::Parser parser((resourceDir / "routes").string());
	ifr::Document parserData("routes1");
	parser.parse((resourceDir / "routes" / "routes1").string(), parserData);

	icarus::http::ValuesHash<icarus::http::Value> values;
	ifr::Piece *piece = parserData.match("GET", "/match/123", values);
	BOOST_REQUIRE_MESSAGE(piece != nullptr, "Could not find a route to match the URI.");
	ifr::Route *route = dynamic_cast<ifr::Route*>(piece);
	BOOST_REQUIRE_MESSAGE(route, "The piece found is not a Route*.");
	BOOST_CHECK_EQUAL(values.size(), 1);
	BOOST_CHECK_EQUAL(values.get("param1"), "123");
}

BOOST_AUTO_TEST_CASE(route1_match2)
{
	namespace ifr = icarus::routes;

	boost::filesystem::path resourceDir(TEST_RESOURCE_DIR);

	boost::filesystem::path routePath;
	ifr::Parser parser((resourceDir / "routes").string());
	ifr::Document parserData("routes1");
	parser.parse((resourceDir / "routes" / "routes1").string(), parserData);

	icarus::http::ValuesHash<icarus::http::Value> values;
	ifr::Piece *piece = parserData.match("POST", "/testing/1/2/test", values);
	BOOST_REQUIRE_MESSAGE(piece == nullptr, "Should not find a route to match the URI.");
}

BOOST_AUTO_TEST_CASE(route1_match3)
{
	namespace ifr = icarus::routes;

	boost::filesystem::path resourceDir(TEST_RESOURCE_DIR);

	boost::filesystem::path routePath;
	ifr::Parser parser((resourceDir / "routes").string());
	ifr::Document parserData("routes1");
	parser.parse((resourceDir / "routes" / "routes1").string(), parserData);

	icarus::http::ValuesHash<icarus::http::Value> values;
	ifr::Piece *piece = parserData.match("PUT", "/testing/1/2/test", values);
	BOOST_REQUIRE_MESSAGE(piece != nullptr, "Could not find a route to match the URI.");
	ifr::Route *route = dynamic_cast<ifr::Route*>(piece);
	BOOST_REQUIRE_MESSAGE(route, "The piece found is not a Route*.");
	BOOST_REQUIRE_EQUAL(values.size(), 2);
	BOOST_CHECK_EQUAL(values.get("count"), "1");
	BOOST_CHECK_EQUAL(values.get("id"), "2");
}

BOOST_AUTO_TEST_CASE(route1_match4)
{
	namespace ifr = icarus::routes;

	boost::filesystem::path resourceDir(TEST_RESOURCE_DIR);

	boost::filesystem::path routePath;
	ifr::Parser parser((resourceDir / "routes").string());
	ifr::Document parserData("routes1");
	parser.parse((resourceDir / "routes" / "routes1").string(), parserData);

	icarus::http::ValuesHash<icarus::http::Value> values;
	ifr::Piece *piece = parserData.match("POST", "/4/foobar", values);
	BOOST_REQUIRE_MESSAGE(piece != nullptr, "Could not find a route to match the URI.");
	ifr::Route *route = dynamic_cast<ifr::Route*>(piece);
	BOOST_REQUIRE_MESSAGE(route, "The piece found is not a Route*.");
	BOOST_REQUIRE_EQUAL(values.size(), 2);
	BOOST_CHECK_EQUAL(values.get("count"), "4");
	BOOST_CHECK_EQUAL(values.get("id"), "foobar");
}
