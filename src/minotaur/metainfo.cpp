
#include <minotaur/metainfo.h>

minotaur::piece::~piece()
{ }

minotaur::content_block::content_block(const std::string &content)
	: content(content)
{ }

minotaur::code_block::code_block(const std::string &content)
	: minotaur::content_block::content_block(content)
{ }

minotaur::quick_code_block::quick_code_block(const std::string &content)
	: minotaur::content_block::content_block(content)
{ }

minotaur::group_block::group_block()
{ }

minotaur::call_code_block::call_code_block(const std::string &name, const std::string &parameters)
	: minotaur::group_block::group_block(), name(name), parameters(parameters)
{ }

void minotaur::group_block::add_piece(minotaur::piece *piece)
{
	this->pieces.emplace_back(piece);
}

void minotaur::file_info::add_param(icarus::routes::method_param *param)
{
	this->parameters.emplace_back(param);
}
