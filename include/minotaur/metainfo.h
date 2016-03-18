/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 17, 2015
 */

#ifndef MINOTAUR_COMPILER_METAINFO_H
#define MINOTAUR_COMPILER_METAINFO_H

#include <string>
#include <vector>
#include <memory>
#include <icarus/routes/data.h>

namespace minotaur
{
class piece
{
public:
	virtual ~piece();
};

class content_block
	: public piece
{
public:
	content_block(const std::string &content);

	std::string content;
};

class code_block
	: public content_block
{
public:
	code_block(const std::string &content);
};

class quick_code_block
	: public content_block
{
public:
	quick_code_block(const std::string &content);
};

class file_info
{
public:
	std::vector<std::unique_ptr<icarus::routes::method_param>> parameters;
	std::vector<std::unique_ptr<piece>> pieces;

	std::string path;
	std::string fullPath;

	std::vector<std::string> package;

	std::string name;
	std::string subextension;
	std::string extension;

	size_t size;
	size_t lines;
	std::string parameters_str;

	void add(minotaur::piece *piece);

	void add(icarus::routes::method_param *param);
};
}

#endif //MINOTAUR_COMPILER_METAINFO_H
