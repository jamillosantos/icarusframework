/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 19, 2015
 */

#include "compiler/compiler.hpp"
#include "compiler/cpptemplatebuilder.hpp"

#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, const char *argv[])
{
	// Declare the supported options.
	po::options_description genericOptions("Generic options");
	genericOptions.add_options()
		("version,v", "version")
		("help,h", "this help message");

	std::string
		inputDirectory,
		outputDirectory;

	po::options_description compilerOptions("Compiler options");
	compilerOptions.add_options()
		("input-dir,i", po::value<std::string>(&inputDirectory)->required(), "input directory")
		("output-dir,o", po::value<std::string>(&outputDirectory)->required(), "output directory")
		("compression,c", "compress the output according to the extension");

	po::options_description options("Allower options:");
	options.add(genericOptions).add(compilerOptions);

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, options), vm);
	try
	{
		po::notify(vm);

		if (vm.count("help"))
		{
			std::cout << options << std::endl;
			return 0;
		}

		minotaur::compiler::Compiler parser(inputDirectory, outputDirectory);

		minotaur::compiler::CppTemplateBuilder templateBuilder;
		// templateBUilder.compress = vm.count("compression");
		parser.compile(templateBuilder);

		return 0;
	}
	catch (boost::program_options::required_option &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
}