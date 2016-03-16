/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date September 09, 2015
 */

#include <icarus/routes/compiler.h>

void icarus::routes::compiler::compile(std::string from, std::string to)
{
	boost::filesystem::path fromPath(from);
	parser parser(fromPath.parent_path().string());
	document document(fromPath.stem().string());
	parser.parse(from, document);

	routes_writer writer;
	std::stringstream memoryStream;
	writer.write(memoryStream, document);

	boost::filesystem::path toPath(to);
	boost::filesystem::create_directories(toPath.parent_path());
	std::ofstream of(toPath.string());
	of << memoryStream.rdbuf();
}

void icarus::routes::compiler::compile(std::string from, std::ostream &to)
{
	boost::filesystem::path fromPath(from);
	parser parser(fromPath.parent_path().string());
	document document(fromPath.stem().string());
	parser.parse(from, document);

	routes_writer writer;
	std::stringstream memoryStream;
	writer.write(memoryStream, document);

	to << memoryStream.rdbuf();
}
