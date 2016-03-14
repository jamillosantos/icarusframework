/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 19, 2015
 */

#ifndef MINOTAUR_COMPILER_EXCEPTIONS_HPP
#define MINOTAUR_COMPILER_EXCEPTIONS_HPP

#include <icarus/exceptions.h>

namespace minotaur
{
namespace compiler
{
class compiler_exception
	: public icarus::exception
{
public:
	compiler_exception(const std::string &message);

	compiler_exception(const std::string &message, std::exception *cause);
};
}
}


#endif // MINOTAUR_COMPILER_EXCEPTIONS_HPP
