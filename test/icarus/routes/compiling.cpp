/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 05, 2016
 **/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE IcarusRoutesCompiling

#include <boost/test/unit_test.hpp>

#include <boost/filesystem/path.hpp>

#include <icarus/routes/compiler.h>
#include <icarus/routes/exceptions.h>

BOOST_AUTO_TEST_CASE(routes1)
{
	namespace ifr = icarus::routes;

	boost::filesystem::path resourceDir(TEST_RESOURCE_DIR);

	boost::filesystem::path routePath;
	ifr::compiler compiler;
	ifr::document document("routes1");
	BOOST_REQUIRE_THROW(compiler.compile((resourceDir / "routes" / "routes1").string(), (resourceDir / "routes" / "generated" / "route1.cpp").string()), icarus::routes::param_not_found);
}

BOOST_AUTO_TEST_CASE(routes2)
{
	namespace ifr = icarus::routes;

	boost::filesystem::path resourceDir(TEST_RESOURCE_DIR);

	boost::filesystem::path routePath;
	ifr::compiler compiler;
	ifr::document document("routes2");
	BOOST_REQUIRE_NO_THROW(compiler.compile((resourceDir / "routes" / "routes2").string(), (resourceDir / "routes" / "generated" / "route2.cpp").string()));
}
