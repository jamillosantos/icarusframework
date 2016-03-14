/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 17, 2015
 */

#ifndef MINOTAUR_COMPILER_METAINFO_H
#define MINOTAUR_COMPILER_METAINFO_H

#include <string>
#include <vector>

namespace minotaur
{
namespace compiler
{
class MetaInfo
{
public:
	std::string path;
	std::string fullPath;

	std::vector<std::string> package;

	std::string name;
	std::string subextension;
	std::string extension;

	size_t size;
	size_t lines;
	std::string parameters;
};
}
}

#endif //MINOTAUR_COMPILER_METAINFO_H
