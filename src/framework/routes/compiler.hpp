/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date September 09, 2015
 */

#ifndef ICARUSFRAMEWORK_ROUTES_COMPILER_HPP
#define ICARUSFRAMEWORK_ROUTES_COMPILER_HPP

namespace icarus
{
namespace routes
{
class Compiler
{
private:
	void compile(std::string from, std::string to)
	{
		Parser parser;
		RoutesData data;
		parser.compile(from, data);

		RoutesWriter writer;

		std::stringstream memoryStream;
		writer.write(memoryStream);
	}
};
}
}

#endif // ICARUSFRAMEWORK_ROUTES_COMPILER_HPP
