/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date September 09, 2015
 */

#ifndef ICARUSFRAMEWORK_ROUTES_COMPILER_HPP
#define ICARUSFRAMEWORK_ROUTES_COMPILER_HPP

#include <sstream>
#include <boost/filesystem.hpp>
#include "parser.hpp"
#include "routeswriter.hpp"
#include "writers/simpleweb.hpp"

namespace icarus
{
namespace framework
{
namespace routes
{
class Compiler
{
public:
	void compile(std::string from, std::string to)
	{
		boost::filesystem::path fromPath(from);
		Parser parser(fromPath.parent_path().string());
		Document document(fromPath.stem().string());
		parser.parse(from, document);

		writers::SimpleWebWriter writer;
		std::stringstream memoryStream;
		writer.write(memoryStream, document);

		boost::filesystem::path toPath(to);
		boost::filesystem::create_directories(toPath.parent_path());
		std::ofstream of(toPath.string());
		of << memoryStream.rdbuf();
	}
};
}
}
}

#endif // ICARUSFRAMEWORK_ROUTES_COMPILER_HPP
