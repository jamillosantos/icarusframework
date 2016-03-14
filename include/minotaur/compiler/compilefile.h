/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 17, 2015
 */

#ifndef ICARUSFRAMEWORK_MINOTAUR_COMPILER_COMPILEFILE_HPP
#define ICARUSFRAMEWORK_MINOTAUR_COMPILER_COMPILEFILE_HPP

#include <vector>
#include <string>
#include <memory>

#include <boost/filesystem.hpp>

#include <fstream>
#include <sstream>
#include <iostream>
#include <boost/regex.hpp>

#include "exceptions.h"
#include "metainfo.h"
#include "templatebuilder.h"

#define INPUT_STREAM_BUFFER_SIZE 4096

namespace minotaur
{
namespace compiler
{
class compile_file
{
public:
	static const boost::regex variableChars;

	MetaInfo info;
private:

	boost::filesystem::path file;

	TemplateBuilder &templateBuilder;

	size_t currentLine;
	size_t currentChar;

	std::unique_ptr<std::ifstream> istream;
	std::ostream &outputStream;

	char inputStreamBuffer[INPUT_STREAM_BUFFER_SIZE];
	size_t inputStreamBufferSize;
	size_t currentInputStreamChar;

	bool initialized;

	char lastChar();

	bool readChar(char *cc);

	void runParameters();

	void runQuotes();

	void runBrackets();

	void runQuickEchoBrackets();

	void runQuickEcho();

	void writeChar(char cc);

public:
	compile_file(boost::filesystem::path file, std::ostream &ostream, TemplateBuilder &templateBuilder);

	void compile();
};

}
}

#endif // ICARUSFRAMEWORK_MINOTAUR_COMPILER_COMPILEFILE_HPP
