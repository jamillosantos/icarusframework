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

#include "exceptions.h"
#include "metainfo.h"
#include "templatebuilder.h"
#include "compilefile.h"

#define INPUT_STREAM_BUFFER_SIZE 4096

namespace minotaur
{
namespace compiler
{
class Compiler
{
private:
	std::vector<std::string> includeDirectories;
	std::string inputFile;
	std::string outputFile;
public:
	Compiler(std::string inputFile, std::string outputFile);

	void compile(TemplateBuilder &templateBuilder);
};
}
}

#endif // ICARUSFRAMEWORK_MINOTAUR_COMPILER_PARSER_H
