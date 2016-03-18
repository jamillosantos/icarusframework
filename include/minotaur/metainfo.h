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

class group_block
	: public piece
{
public:
	group_block();

	std::vector<std::unique_ptr<piece>> pieces;

	void add_piece(minotaur::piece *piece);
};

class call_code_block
	: public group_block
{
public:
	call_code_block(const std::string &name, const std::string &parameters);

	std::string name;
	std::string parameters;
};

class file_info
	: public group_block
{
public:
	std::vector<std::unique_ptr<icarus::routes::method_param>> parameters;

	std::string path;
	std::string fullPath;

	std::vector<std::string> package;

	std::string name;
	std::string subextension;
	std::string extension;

	size_t size;
	size_t lines;
	std::string parameters_str;

	void add_param(icarus::routes::method_param *param);
};
}

#endif //MINOTAUR_COMPILER_METAINFO_H
