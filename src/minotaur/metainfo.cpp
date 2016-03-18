
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

void minotaur::file_info::add(minotaur::piece *piece)
{
	this->pieces.emplace_back(piece);
}

void minotaur::file_info::add(icarus::routes::method_param *param)
{
	this->parameters.emplace_back(param);
}
