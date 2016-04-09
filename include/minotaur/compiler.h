/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 17, 2015
 */

#ifndef ICARUSFRAMEWORK_MINOTAUR_COMPILER_PARSER_H
#define ICARUSFRAMEWORK_MINOTAUR_COMPILER_PARSER_H

#include <vector>
#include <string>
#include <memory>

#include <boost/filesystem.hpp>
#include <fstream>
#include <sstream>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

#include <minotaur/exceptions.h>
#include <minotaur/metainfo.h>
#include <minotaur/templatebuilder.h>
#include <minotaur/parser.h>

namespace minotaur
{
class compiler
{
private:
	std::string input_file;
	std::string output_file;

	bool needs_update(const boost::filesystem::path &source, const boost::filesystem::path &compiled);
public:
	compiler(std::string inputFile, std::string outputFile);

	void compile(minotaur::template_builder &builder);
};
}

#endif // ICARUSFRAMEWORK_MINOTAUR_COMPILER_PARSER_H
