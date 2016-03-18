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

	minotaur::parser_file parser(in_stream);
	BOOST_CHECK_NO_THROW(parser.parse(finfo));

	BOOST_REQUIRE_GT(finfo.pieces.size(), 0);

	BOOST_CHECK_EQUAL(dynamic_cast<minotaur::content_block*>(finfo.pieces[0].get())->content, (
		"<!DOCTYPE html>\n"
		"<html lang=\"en\">\n"
		"<head>\n"
		"\t<meta charset=\"UTF-8\">\n"
		"\t<title>Testing "
	));

	BOOST_REQUIRE_GT(finfo.pieces.size(), 1);
	BOOST_CHECK_EQUAL(dynamic_cast<minotaur::content_block*>(finfo.pieces[1].get())->content, "count");

	BOOST_REQUIRE_GT(finfo.pieces.size(), 2);
	BOOST_CHECK_EQUAL(dynamic_cast<minotaur::content_block*>(finfo.pieces[2].get())->content, "</title>\n"
		"</head>\n"
		"<body>\n");

	BOOST_REQUIRE_GT(finfo.pieces.size(), 3);
	BOOST_CHECK_EQUAL(dynamic_cast<minotaur::content_block*>(finfo.pieces[3].get())->content, "count");

	BOOST_REQUIRE_GT(finfo.pieces.size(), 4);
	BOOST_CHECK_EQUAL(dynamic_cast<minotaur::content_block*>(finfo.pieces[4].get())->content, "\n");

	BOOST_REQUIRE_GT(finfo.pieces.size(), 5);
	BOOST_CHECK_EQUAL(dynamic_cast<minotaur::content_block*>(finfo.pieces[5].get())->content, "\n"
		"\tfor (unsigned int i = 0; i < count; i++)\n"
		"\t{\n"
		"\t\tout << i << \"<br />\" << std::endl;\n"
		"\t}\n");

	BOOST_REQUIRE_GT(finfo.pieces.size(), 6);
	BOOST_CHECK_EQUAL(dynamic_cast<minotaur::content_block*>(finfo.pieces[6].get())->content, "\n\n");

	BOOST_REQUIRE_GT(finfo.pieces.size(), 7);
	BOOST_CHECK_EQUAL(dynamic_cast<minotaur::call_code_block*>(finfo.pieces[7].get())->name, "templates::view");
	BOOST_CHECK_EQUAL(dynamic_cast<minotaur::call_code_block*>(finfo.pieces[7].get())->parameters, "\"Testing a param\"");
	BOOST_REQUIRE_EQUAL(dynamic_cast<minotaur::call_code_block*>(finfo.pieces[7].get())->pieces.size(), 1);
	BOOST_REQUIRE(dynamic_cast<minotaur::call_code_block*>(finfo.pieces[7].get())->pieces[0]);
	/*
	BOOST_CHECK_EQUAL(dynamic_cast<minotaur::content_block*>(dynamic_cast<minotaur::call_code_block*>(finfo.pieces[7].get())->pieces[0].get())->content, "\n"
		"\ttesting a new content.");
	*/

	BOOST_REQUIRE_GT(finfo.pieces.size(), 8);
	BOOST_CHECK_EQUAL(dynamic_cast<minotaur::content_block*>(finfo.pieces[8].get())->content, "\n"
		"\n"
		"</body>\n"
		"</html>");
}

