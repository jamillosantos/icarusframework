/**
 * @author J. Santos <jamillo@gmail.com>
 * @date April 08, 2016
 */


#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE digests

#include <boost/test/unit_test.hpp>
#include <icarus/digests.h>

BOOST_AUTO_TEST_CASE(sha_256)
{
	BOOST_CHECK_EQUAL(icarus::digests::sha256("12345"), "5994471abb01112afcc18159f6cc74b4f511b99806da59b3caf5a9c173cacfc5");
}
