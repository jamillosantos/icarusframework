/**
 * @author J. Santos <jamillo@gmail.com>
 * @date March 14, 2016
 **/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE content

#include <boost/test/unit_test.hpp>
#include <icarus/content.h>

BOOST_AUTO_TEST_CASE(append_1)
{
	icarus::content content;
	content << "testing content";
	BOOST_CHECK_EQUAL(content.size(), 15);
}
