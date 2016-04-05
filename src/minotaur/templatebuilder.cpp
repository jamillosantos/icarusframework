
#include <minotaur/templatebuilder.h>

#include <icarus/exceptions.h>

minotaur::content_info::content_info(const std::string &stream, unsigned int level)
	: stream(stream), level(level)
{ }

minotaur::template_builder::template_builder()
{ }

void minotaur::template_builder::document_begin(file_info &fileInfo, std::ostream &ostream)
{ }

void minotaur::template_builder::document_end(file_info &fileInfo, std::ostream &ostream)
{ }

void minotaur::template_builder::class_begin(file_info &fileInfo, std::ostream &ostream)
{ }

void minotaur::template_builder::class_end(file_info &fileInfo, std::ostream &ostream)
{ }

void minotaur::template_builder::write(minotaur::content_info &info, minotaur::piece *piece, std::ostream &ostream)
{
	minotaur::code_block *code_block;
	minotaur::quick_code_block *quick_code_block;
	minotaur::content_block *cont_block;
	minotaur::call_code_block *call_code_block;
	minotaur::group_block *group_block;
	if ((call_code_block = dynamic_cast<minotaur::call_code_block*>(piece)))
		this->write(info, *call_code_block, ostream);
	else if ((group_block = dynamic_cast<minotaur::group_block*>(piece)))
		this->write(info, *group_block, ostream);
	else if ((code_block = dynamic_cast<minotaur::code_block*>(piece)))
		this->write(info, *code_block, ostream);
	else if ((quick_code_block = dynamic_cast<minotaur::quick_code_block*>(piece)))
		this->write(info, *quick_code_block, ostream);
	else if ((cont_block = dynamic_cast<minotaur::content_block*>(piece)))
		this->write(info, *cont_block, ostream);
	else
		throw icarus::exception("Piece not implemented.");
}

void minotaur::template_builder::write(minotaur::content_info &info, minotaur::group_block &group, std::ostream &ostream)
{ }

void minotaur::template_builder::write(minotaur::content_info &info, minotaur::content_block &content, std::ostream &ostream)
{ }

void minotaur::template_builder::write(minotaur::content_info &info, minotaur::code_block &code, std::ostream &ostream)
{ }

void minotaur::template_builder::write(minotaur::content_info &info, minotaur::quick_code_block &code, std::ostream &ostream)
{ }

void minotaur::template_builder::write(minotaur::content_info &info, minotaur::call_code_block &code, std::ostream &ostream)
{ }

void minotaur::template_builder::write(minotaur::file_info &finfo, std::ostream &ostream)
{
	this->document_begin(finfo, ostream);
	this->class_begin(finfo, ostream);
	minotaur::content_info info("out", 0);
	for (const std::unique_ptr<minotaur::piece> &piece : finfo.pieces)
		this->write(info, piece.get(), ostream);
	this->class_end(finfo, ostream);
	this->document_end(finfo, ostream);
}
