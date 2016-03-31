/**
 * @author J. Santos <jamillo@gmail.com>
 * @date March 15, 2016
 **/

#include <icarus/routes/data.h>
#include <icarus/routes/compiler.h>

#include <boost/program_options.hpp>

int main(int argc, char **argv)
{
	namespace ifr = icarus::routes;

	std::string
		in_routes,
		out_routes;

	boost::program_options::options_description commandLineOnlyOptions("Command Line Options");

	commandLineOnlyOptions.add_options()
		("version,v", "print version string")
		("help,h", "print help message")
		("input,i", boost::program_options::value<std::string>(&in_routes)->required(), "input route file")
		("output,o", boost::program_options::value<std::string>(&out_routes)->default_value("-"), "output route file ('-' for standard output)");

	boost::program_options::variables_map vm;
	boost::program_options::store(boost::program_options::command_line_parser(argc, argv).options(commandLineOnlyOptions).run(), vm);
	boost::program_options::notify(vm);

	if (vm.count("version"))
	{
		std::cout << "Version: " << VERSION_MAJOR << "." << VERSION_MINOR << "." << VERSION_PATCH << "." <<
		VERSION_TWEAK << std::endl;
	}
	else
	{
		ifr::compiler compiler;

		if (out_routes == "-")
		{
			boost::filesystem::path in(in_routes);
			ifr::document document(in.stem().string());
			compiler.compile(in_routes, std::cout);
		}
		else
		{
			boost::filesystem::path o(out_routes);
			ifr::document document(o.stem().string());
			compiler.compile(in_routes, out_routes);
		}
	}
	return 0;
}