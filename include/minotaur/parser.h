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

#include <sstream>
#include <iostream>
#include <boost/regex.hpp>

#include <minotaur/exceptions.h>
#include <minotaur/metainfo.h>
#include <minotaur/templatebuilder.h>

#define INPUT_STREAM_BUFFER_SIZE 4096

namespace minotaur
{
class parser_file
{
private:
	std::istream &istream;

	boost::filesystem::path file;

	size_t currentLine;
	size_t currentChar;

	char inputStreamBuffer[INPUT_STREAM_BUFFER_SIZE];
	size_t inputStreamBufferSize;
	size_t currentInputStreamChar;

	bool initialized;

	char read_next;

	char last_char();

	bool read(char *cc);

	std::string read_until_non_blank(char *cc);

	std::string read_until(std::string chars);

	void run_parameters(minotaur::file_info &file);

	void run_content(minotaur::group_block &block, bool skip_at = false, bool root = false);

	void run_block_code(minotaur::group_block &block);

	void run_quotes(std::stringstream &block);

	void run_single_quotes(std::stringstream &block);

	void run_brackets(std::stringstream &block);

	void run_quick_echo_brackets(std::stringstream &sblock);

	void run_quick_echo(minotaur::group_block &block);

public:
	parser_file(std::istream &in_stream);

	void parse(minotaur::file_info &file);
};
}

#endif // ICARUSFRAMEWORK_MINOTAUR_COMPILER_COMPILEFILE_HPP
