/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#ifndef ICARUSFRAMEWORK_ROUTES_PARSER_HPP
#define ICARUSFRAMEWORK_ROUTES_PARSER_HPP

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

	char lastChar();

	bool readChar(char *cc);

	void readUntilNonBlank(char *cc);

	std::string readUntil(std::string chars);

	/**
	 * Reads bytes untilfind the end of the comment.
	 */
	void runCommentLine();

	/**
	 * Reads bytes until find the end of the comment.
	 */
	void runCommentMultiLine();

	std::string runSkipQuotes();

	std::string runSkipRegexBrackets();

	void runLineMethodParameters(call_method &callMethod);

	char readURI(composed_uri &uri);

	void runGroup(routes &data, unsigned int level);

	void runLine(routes &data, unsigned int level);

	void runDoc(routes &data, unsigned int level);
public:
	parser(std::string inputFolder);

	void parse(std::string inputFile, document &data);
};
}
}
#endif //ICARUSFRAMEWORK_ROUTES_PARSER_HPP
