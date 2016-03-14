
#include <minotaur/compiler/templatebuilder.h>

minotaur::compiler::TemplateBuilder::TemplateBuilder()
	: _document_level(0), _class_level(0), _string_block_level(0)
{ }

void minotaur::compiler::TemplateBuilder::document_begin(MetaInfo &fileInfo, std::ostream &ostream)
{
	this->_document_level++;
}

void minotaur::compiler::TemplateBuilder::document_end(MetaInfo &fileInfo, std::ostream &ostream)
{
	this->_document_level--;
}

const int minotaur::compiler::TemplateBuilder::document_level()
{
	return this->_document_level;
}

void minotaur::compiler::TemplateBuilder::class_begin(MetaInfo &fileInfo, std::ostream &ostream)
{
	this->_class_level++;
}

void minotaur::compiler::TemplateBuilder::class_end(MetaInfo &fileInfo, std::ostream &ostream)
{
	this->_class_level--;
}

const int minotaur::compiler::TemplateBuilder::class_level()
{
	return this->_class_level;
}

void minotaur::compiler::TemplateBuilder::string_block_begin(MetaInfo &fileInfo, std::ostream &ostream)
{
	this->_string_block_level++;
}

void minotaur::compiler::TemplateBuilder::string_block_end(MetaInfo &fileInfo, std::ostream &ostream)
{
	this->_string_block_level--;
}

const int minotaur::compiler::TemplateBuilder::string_block_level()
{
	return this->_string_block_level;
}

void minotaur::compiler::TemplateBuilder::write_block_begin(MetaInfo &fileInfo, std::ostream &ostream)
{
	this->_write_block_level++;
}

void minotaur::compiler::TemplateBuilder::write_block_end(MetaInfo &fileInfo, std::ostream &ostream)
{
	this->_write_block_level--;
}

const int minotaur::compiler::TemplateBuilder::write_block_level()
{
	return this->_write_block_level;
}

void minotaur::compiler::TemplateBuilder::write(MetaInfo &fileInfo, std::ostream &ostream, char c)
{
	ostream << c;
}