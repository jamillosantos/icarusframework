#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE routes2

#include <boost/test/included/unit_test.hpp>

#include "../../../icarus/routes/parser.hpp"
#include "../../../icarus/routes/compiler.hpp"

BOOST_AUTO_TEST_CASE(route2_parsing)
{
	namespace ifr = icarus::routes;

	boost::filesystem::path resourceDir(TEST_RESOURCE_DIR);

	boost::filesystem::path routePath;
	ifr::Parser parser((resourceDir / "routes").string());
	ifr::Document parserData("routes2");
	parser.parse((resourceDir / "routes" / "routes2").string(), parserData);

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
		BOOST_CHECK_EQUAL(line.callMethod().params()[0].name(), "param1");
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
		BOOST_CHECK_EQUAL(line.uri().tokens()[1].regex(), ifr::fieldTypes.get("uint8_t"));
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
		BOOST_CHECK_EQUAL(line.uri().tokens()[1].regex(), "");
		BOOST_CHECK(line.uri().tokens()[2].name().empty());
		BOOST_CHECK_EQUAL(line.uri().tokens()[2].regex(), "/");
		BOOST_CHECK_EQUAL(line.uri().tokens()[3].name(), "id");
		BOOST_CHECK_EQUAL(line.uri().tokens()[3].regex(), ifr::fieldTypes.get("uint8_t"));
		BOOST_CHECK(line.uri().tokens()[4].name().empty());
		BOOST_CHECK_EQUAL(line.uri().tokens()[4].regex(), "/test");

		BOOST_REQUIRE_EQUAL(line.callMethod().path().size(), 2);
		BOOST_CHECK_EQUAL(line.callMethod().path()[0], "controllers");
		BOOST_CHECK_EQUAL(line.callMethod().path()[1], "Index");
		BOOST_CHECK_EQUAL(line.callMethod().name(), "default4");
		BOOST_REQUIRE_EQUAL(line.callMethod().params().size(), 2);
		BOOST_CHECK_EQUAL(line.callMethod().params()[0].name(), "id");
		BOOST_CHECK_EQUAL(line.callMethod().params()[0].type(), "uint8_t");
		BOOST_CHECK_EQUAL(line.callMethod().params()[1].name(), "count");
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

BOOST_AUTO_TEST_CASE(route2_compiling)
{
	namespace ifr = icarus::routes;

	boost::filesystem::path resourceDir(TEST_RESOURCE_DIR);

	boost::filesystem::path routePath;
	ifr::Compiler compiler;
	ifr::Document document("routes2");
	compiler.compile((resourceDir / "routes" / "routes2").string(), (resourceDir / "routes" / "generated" / "route2.cpp").string());
}
