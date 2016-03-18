
#include <minotaur/exceptions.h>

minotaur::compiler_exception::compiler_exception(const unsigned int line, const std::string &message)
	: icarus::exception(message + " At line " + std::to_string(line))
{ }

minotaur::compiler_exception::compiler_exception(const unsigned int line, const std::string &message, std::exception *cause)
	: icarus::exception(message + " At line " + std::to_string(line), cause)
{ }
