
#include <minotaur/compiler/exceptions.h>

minotaur::compiler::compiler_exception::compiler_exception(const std::string &message)
	: icarus::exception(message)
{ }

minotaur::compiler::compiler_exception::compiler_exception(const std::string &message, std::exception *cause)
	: icarus::exception(message, cause)
{ }
