/**
 * @author J. Santos <jamillo@gmail.com>
 * @date March 14, 2016
 **/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE content

#include <boost/test/unit_test.hpp>
#include <icarus/content.h>
#include <icarus/exceptions.h>

BOOST_AUTO_TEST_CASE(append_1)
{
	icarus::content content;
	content << "testing content";
	BOOST_CHECK_EQUAL(content.size(), 15);
}

BOOST_AUTO_TEST_CASE(copy)
{
	icarus::content content;
	content << "testing content";
	icarus::content content_copy(content);
	BOOST_CHECK_EQUAL(content.size(), 15);
	BOOST_CHECK_EQUAL(content_copy.size(), 15);
	content_copy << "testing content";
	BOOST_CHECK_EQUAL(content.size(), 15);
	BOOST_CHECK_EQUAL(content_copy.size(), 30);
}

BOOST_AUTO_TEST_CASE(move)
{
	icarus::content content;
	content << "testing content";
	BOOST_CHECK_EQUAL(content.size(), 15);
	icarus::content content_move(std::move(content));
	BOOST_CHECK_EQUAL(content_move.size(), 15);
	content_move << "testing content";
	BOOST_CHECK_THROW(content.size(), icarus::invalid_pointer);
	BOOST_CHECK_EQUAL(content_move.size(), 30);
}
