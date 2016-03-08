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
			if (boost::filesystem::is_directory(id))
			{
				boost::filesystem::recursive_directory_iterator dirEnd;
				for (boost::filesystem::recursive_directory_iterator d(id);
					 d != dirEnd; ++d)
				{
					std::cout << "*" << d->path().string();
					if (boost::filesystem::is_regular(*d))
					{
						std::cout << ". OK";
						std::string extension = d->path().extension().string();
						parentPath = d->path().parent_path().string();

						std::stringstream ostr;
						CompileFile c((*d).path(), ostr, templateBuilder);

						std::string relativePath;
						if (parentPath != idFullPath)
							relativePath = parentPath.substr(idFullPath.length()+1);

						boost::filesystem::path targetCppFile(od.string() + boost::filesystem::path::preferred_separator
							+ (relativePath) + boost::filesystem::path::preferred_separator + c.info.name
							+ c.info.extension + ".hpp"
						);
						boost::filesystem::create_directories(targetCppFile.parent_path().string());

						boost::split(c.info.package, relativePath, boost::is_any_of("\\/"));
						c.compile();

						std::ofstream ostream(targetCppFile.string());
						if (ostream)
							ostream << ostr.rdbuf();
						else
						{
							// TODO: specialize exception
							std::cerr << "Cannot create file stream on " << targetCppFile.string() << "." << std::endl;
							throw std::exception();
						}
					}
					std::cout << std::endl;
				}
			}
			else if (boost::filesystem::is_regular_file(id))
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

#endif // ICARUSFRAMEWORK_MINOTAUR_COMPILER_PARSER_H
