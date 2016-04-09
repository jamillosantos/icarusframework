/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date September 09, 2015
 */

#include <icarus/routes/compiler.h>

void icarus::routes::compiler::compile(std::string from, std::string to)
{
	boost::filesystem::path from_path(from);
	boost::filesystem::path to_path(to);

	if (
		(!boost::filesystem::exists(to_path))
		|| (boost::filesystem::last_write_time(from_path) > boost::filesystem::last_write_time(to_path))
	)
	{
		parser parser(from_path.parent_path().string());
		document document(from_path.stem().string());
		parser.parse(from, document);

		routes_writer writer;
		std::stringstream memoryStream;
		writer.write(memoryStream, document);

		boost::filesystem::create_directories(to_path.parent_path());
		std::ofstream of(to_path.string());
		of << memoryStream.rdbuf();
	}
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
