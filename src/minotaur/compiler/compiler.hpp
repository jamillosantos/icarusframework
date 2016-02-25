/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 17, 2015
 */

#ifndef ICARUSFRAMEWORK_MINOTAUROS_COMPILER_PARSER_H
#define ICARUSFRAMEWORK_MINOTAUROS_COMPILER_PARSER_H

#include <vector>
#include <string>
#include <memory>

#include <boost/filesystem.hpp>
#include <fstream>
#include <sstream>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>

#include "exceptions.hpp"
#include "metainfo.hpp"
#include "templatebuilder.hpp"
#include "compilefile.hpp"

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
	Compiler(std::string inputFile, std::string outputFile)
		: inputFile(inputFile), outputFile(outputFile)
	{ }

	void compile(TemplateBuilder &templateBuilder) throw(CompilerException)
	{
		boost::filesystem::path id(this->inputFile);
		boost::filesystem::path od(this->outputFile);

		std::string
			idFullPath = id.string(),
			parentPath;

		std::vector<std::string> currentStack;
		if (boost::filesystem::exists(id))
		{
			if (boost::filesystem::is_regular_file(id))
			{
				std::string extension = id.extension().string();
				parentPath = id.parent_path().string();

				std::stringstream ostr;
				CompileFile c(id, ostr, templateBuilder);
				c.compile();

				std::cout << od.string() << std::endl;
				std::ofstream ostream(od.string());
				if (ostream)
					ostream << ostr.rdbuf();
				else
				{
					// TODO: specialize exception
					std::cerr << "Cannot create file stream on " << od.string() << "." << std::endl;
					throw std::exception();
				}
			}
			else
			{
				// TODO: Throw an exception.
			}
		}
		else
		{
			// TODO: Throw an exception.
		}
	}
};
}
}

#endif // ICARUSFRAMEWORK_MINOTAUROS_COMPILER_PARSER_H
