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
class Parser
{
private:
	std::vector<std::string> includeDirectories;
	std::string inputDirectory;
	std::string outputDirectory;
public:
	Parser(std::string inputDirectory, std::string outputDirectory)
		: inputDirectory(inputDirectory), outputDirectory(outputDirectory)
	{ }

	void addIncludeDirectory(const std::string &str)
	{
		this->includeDirectories.push_back(str);
	}

	const std::string &getOutputDirectory() const
	{
		return outputDirectory;
	}

	void setOutputDirectory(const std::string &outputDirectory)
	{
		Parser::outputDirectory = outputDirectory;
	}

	void compile(TemplateBuilder &templateBuilder) throw(CompilerException)
	{
		boost::filesystem::path id(this->inputDirectory);
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
					if (boost::filesystem::is_regular(*d))
					{
						std::cout << "Compiling " << d->path().string();

						parentPath = d->path().parent_path().string();

						std::stringstream ostr;
						CompileFile c((*d).path(), ostr, templateBuilder);
						if (parentPath != idFullPath)
						{
							std::string relativePath = parentPath.substr(idFullPath.length()+1);
							boost::split(c.info.package, relativePath, boost::is_any_of("\\/"));
						}
						c.compile();

						std::cout << " to: " << std::endl;
						std::cout << ostr.str() << std::endl;
					}
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
