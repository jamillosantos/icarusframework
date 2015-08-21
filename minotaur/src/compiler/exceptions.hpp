/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 19, 2015
 */

#ifndef MINOTAUR_COMPILER_EXCEPTIONS_HPP
#define MINOTAUR_COMPILER_EXCEPTIONS_HPP

#include <exception>

namespace minotaur
{
namespace compiler
{
class CompilerException
	: public std::exception
{ };
}
}


#endif // MINOTAUR_COMPILER_EXCEPTIONS_HPP
