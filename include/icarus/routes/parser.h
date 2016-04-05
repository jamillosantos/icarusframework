/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#ifndef ICARUSFRAMEWORK_ROUTES_PARSER_H
#define ICARUSFRAMEWORK_ROUTES_PARSER_H

#define INPUT_STREAM_BUFFER_SIZE 4096

#include <string>
#include <stdexcept>
#include <fstream>
#include <sstream>

#include <boost/filesystem.hpp>
#include <boost/locale.hpp>
#include <boost/regex.hpp>

#include <icarus/routes/data.h>

namespace icarus
{
namespace routes
{
class parser
{
private:
	static boost::regex variablesName;

	std::unique_ptr<std::istream> istream;

	std::string inputFolder;

	char inputStreamBuffer[INPUT_STREAM_BUFFER_SIZE];
	size_t inputStreamBufferSize;
	size_t currentInputStreamChar;

	size_t currentLine;
	size_t currentChar;

	char _next_char;

	char last_char();

	bool read_char(char *cc);

	void read_until_non_blank(char *cc);

	std::string read_until(std::string chars);

	std::string read_identifier();

	/**
	 * Reads bytes untilfind the end of the comment.
	 */
	void run_comment_line();

	/**
	 * Reads bytes until find the end of the comment.
	 */
	void run_comment_multiline();

	std::string run_skip_quotes();

	std::string run_skip_regex_brackets();

	void run_line_method_parameters(call_method &callMethod);

	char read_uri(composed_uri &uri);

	void run_group(routes &data, unsigned int level);

	void run_line(routes &data, unsigned int level);

	void run_doc(routes &data, unsigned int level);
public:
	parser(std::string inputFolder);

	void parse(std::string inputFile, document &data);
};
}
}
#endif //ICARUSFRAMEWORK_ROUTES_PARSER_H
