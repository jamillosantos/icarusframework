
#include <minotaur/templatebuilder.h>

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

void minotaur::template_builder::write(minotaur::content_block &content, std::ostream &ostream)
{ }

void minotaur::template_builder::write(minotaur::code_block &code, std::ostream &ostream)
{ }

void minotaur::template_builder::write(minotaur::quick_code_block &code, std::ostream &ostream)
{ }

void minotaur::template_builder::write(minotaur::file_info &finfo, std::ostream &ostream)
{
	this->document_begin(finfo, ostream);
	this->class_begin(finfo, ostream);
	minotaur::code_block *code_block;
	minotaur::quick_code_block *quick_code_block;
	minotaur::content_block *cont_block;
	for (const std::unique_ptr<minotaur::piece> &piece : finfo.pieces)
	{
		if ((code_block = dynamic_cast<minotaur::code_block*>(piece.get())))
			this->write(*code_block, ostream);
		if ((quick_code_block = dynamic_cast<minotaur::quick_code_block*>(piece.get())))
			this->write(*quick_code_block, ostream);
		else if ((cont_block = dynamic_cast<minotaur::content_block*>(piece.get())))
			this->write(*cont_block, ostream);
		else
			throw icarus::exception("Piece not implemented.");
	}
	this->class_end(finfo, ostream);
	this->document_end(finfo, ostream);
}