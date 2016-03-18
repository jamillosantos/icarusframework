/**
 * @author J. Santos <jamillo@gmail.com>
 * @date March 14, 2016
 **/

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE content

#include <boost/test/unit_test.hpp>
#include <minotaur/parser.h>

BOOST_AUTO_TEST_CASE(parse_1)
{
	boost::filesystem::path resource(TEST_RESOURCE_DIR);

	std::ifstream in_stream((resource / "minotaur" / "templates" / "sample.cpp.html").string());
	minotaur::file_info finfo;

	BOOST_REQUIRE(in_stream);

	minotaur::parse_file parser(finfo, in_stream);
	BOOST_CHECK_NO_THROW(parser.parse());

	BOOST_REQUIRE_EQUAL(finfo.pieces.size(), 7);

	BOOST_CHECK_EQUAL(dynamic_cast<minotaur::content_block*>(finfo.pieces[0].get())->content, ("\n"
		"<!DOCTYPE html>\n"
		"<html lang=\"en\">\n"
		"<head>\n"
		"\t<meta charset=\"UTF-8\">\n"
		"\t<title>Testing "));

	BOOST_CHECK_EQUAL(dynamic_cast<minotaur::content_block*>(finfo.pieces[1].get())->content, "count");

	BOOST_CHECK_EQUAL(dynamic_cast<minotaur::content_block*>(finfo.pieces[2].get())->content, "</title>\n"
		"</head>\n"
		"<body>\n");

	BOOST_CHECK_EQUAL(dynamic_cast<minotaur::content_block*>(finfo.pieces[3].get())->content, "count");

	BOOST_CHECK_EQUAL(dynamic_cast<minotaur::content_block*>(finfo.pieces[4].get())->content, "\n");

	BOOST_CHECK_EQUAL(dynamic_cast<minotaur::content_block*>(finfo.pieces[5].get())->content, "\n"
		"\tfor (unsigned int i = 0; i < count; i++)\n"
		"\t{\n"
		"\t\tout << i << \"<br />\" << std::endl;\n"
		"\t}\n");

	BOOST_CHECK_EQUAL(dynamic_cast<minotaur::content_block*>(finfo.pieces[6].get())->content, "\n"
		"\n"
		"</body>\n"
		"</html>");
}

