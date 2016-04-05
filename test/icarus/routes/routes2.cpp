
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE routes2

#include <boost/test/unit_test.hpp>

#include <icarus/routes/fieldtypes.h>

#include <icarus/routes/parser.h>
#include <icarus/routes/compiler.h>

BOOST_AUTO_TEST_CASE(parsing)
{
	namespace ifr = icarus::routes;

	boost::filesystem::path resourceDir(TEST_RESOURCE_DIR);

	boost::filesystem::path routePath;
	ifr::parser parser((resourceDir / "routes").string());
	ifr::document parserData("routes2");
	parser.parse((resourceDir / "routes" / "routes2").string(), parserData);

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
		BOOST_REQUIRE_EQUAL(line.call_method().params().size(), 1);
		BOOST_CHECK(line.call_method().params()[0].attribute() == icarus::routes::method_param_type::IDENTIFIER);
		BOOST_CHECK_EQUAL(line.call_method().params()[0].name(), "client");
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
		BOOST_CHECK_EQUAL(line.call_method().params()[0].name(), "param1");
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
		BOOST_CHECK_EQUAL(line.uri().tokens()[1].regex(), ifr::field_types::types.get("uint8_t"));
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
		BOOST_CHECK_EQUAL(line.uri().tokens()[1].regex(), "");
		BOOST_CHECK(line.uri().tokens()[2].name().empty());
		BOOST_CHECK_EQUAL(line.uri().tokens()[2].regex(), "/");
		BOOST_CHECK_EQUAL(line.uri().tokens()[3].name(), "id");
		BOOST_CHECK_EQUAL(line.uri().tokens()[3].regex(), ifr::field_types::types.get("uint8_t"));
		BOOST_CHECK(line.uri().tokens()[4].name().empty());
		BOOST_CHECK_EQUAL(line.uri().tokens()[4].regex(), "/test");

		BOOST_REQUIRE_EQUAL(line.call_method().path().size(), 2);
		BOOST_CHECK_EQUAL(line.call_method().path()[0], "controllers");
		BOOST_CHECK_EQUAL(line.call_method().path()[1], "Index");
		BOOST_CHECK_EQUAL(line.call_method().name(), "default4");
		BOOST_REQUIRE_EQUAL(line.call_method().params().size(), 2);
		BOOST_CHECK_EQUAL(line.call_method().params()[0].name(), "id");
		BOOST_CHECK_EQUAL(line.call_method().params()[0].type(), "uint8_t");
		BOOST_CHECK_EQUAL(line.call_method().params()[1].name(), "count");
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

BOOST_AUTO_TEST_CASE(matching_root)
{
	namespace ifr = icarus::routes;

	icarus::routes::route route(0, "GET", {
		{"", "/"}
	});
	std::vector<std::string> values;
	BOOST_CHECK(route.match("GET", "/", values));
	BOOST_CHECK_EQUAL(values.size(), 0);
	BOOST_CHECK(!route.match("GET", "/test", values));
	BOOST_CHECK_EQUAL(values.size(), 0);
	BOOST_CHECK(!route.match("POST", "/", values));
	BOOST_CHECK_EQUAL(values.size(), 0);
}

BOOST_AUTO_TEST_CASE(matching_param)
{
	namespace ifr = icarus::routes;

	icarus::routes::route route(0, "GET", {
		{"", "/"},
		{"user", "[^/]+"},
		{"", "/"}
	});

	{
		std::vector<std::string> values;
		BOOST_CHECK(!route.match("GET", "/", values));
		BOOST_CHECK_EQUAL(values.size(), 0);
	}
	{
		std::vector<std::string> values;
		BOOST_CHECK(route.match("GET", "/username/", values));
		BOOST_CHECK_EQUAL(values.size(), 1);
		BOOST_CHECK_EQUAL(values[0], "username");
	}
	{
		std::vector<std::string> values;
		BOOST_CHECK(route.match("GET", "/ze/", values));
		BOOST_CHECK_EQUAL(values.size(), 1);
		BOOST_CHECK_EQUAL(values[0], "ze");
	}
	{
		std::vector<std::string> values;
		BOOST_CHECK(!route.match("POST", "/username/", values));
		BOOST_CHECK_EQUAL(values.size(), 0);
	}
}

BOOST_AUTO_TEST_CASE(route2_compiling)
{
	namespace ifr = icarus::routes;

	boost::filesystem::path resourceDir(TEST_RESOURCE_DIR);

	boost::filesystem::path routePath;
	ifr::compiler compiler;
	ifr::document document("routes2");
	compiler.compile((resourceDir / "routes" / "routes2").string(), (resourceDir / "routes" / "generated" / "route2.cpp").string());
}
