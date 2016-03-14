/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 19, 2015
 */

#include "minotaur/compiler/compiler.cpp"
#include "minotaur/compiler/cpptemplatebuilder.cpp"

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
		inputFile,
		outputFile;

	po::options_description compilerOptions("Compiler options");
	compilerOptions.add_options()
		("input,i", po::value<std::string>(&inputFile)->required(), "input file")
		("output,o", po::value<std::string>(&outputFile)->required(), "output file")
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

		minotaur::compiler::Compiler parser(inputFile, outputFile);
		minotaur::compiler::CppTemplateBuilder templateBuilder;
		parser.compile(templateBuilder);

		return 0;
	}
	catch (boost::program_options::required_option &e)
	{
		std::cerr << e.what() << std::endl;
		return 1;
	}
}