/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 19, 2015
 */

#include "compiler/parser.hpp"
#include "compiler/cpptemplatebuilder.hpp"

int main()
{
	minotaur::compiler::Parser parser(
		"/home/jsantos/projects/icarusframework/minotaur/src/test/templates",
		"/home/jsantos/projects/icarusframework/minotaur/src/test/templates.generated"
	);
	minotaur::compiler::CppTemplateBuilder templateBuilder;
	parser.compile(templateBuilder);

	return 0;
}