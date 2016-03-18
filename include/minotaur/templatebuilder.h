/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 19, 2015
 */

#ifndef ICARUSFRAMEWORK_MINOTAUR_COMPILER_TEMPLATEBUILDER_H
#define ICARUSFRAMEWORK_MINOTAUR_COMPILER_TEMPLATEBUILDER_H

#include <iostream>

#include <minotaur/metainfo.h>

namespace minotaur
{
class content_info
{
public:
	content_info(const std::string &stream, unsigned int level);

	std::string stream;
	unsigned int level;
};

class template_builder
{
protected:
	virtual void document_begin(file_info &fileInfo, std::ostream &ostream);

	virtual void document_end(file_info &fileInfo, std::ostream &ostream);

	virtual void class_begin(file_info &fileInfo, std::ostream &ostream);

	virtual void class_end(file_info &fileInfo, std::ostream &ostream);

	virtual void write(minotaur::content_info &info, minotaur::piece *piece, std::ostream &ostream);

	virtual void write(minotaur::content_info &info, minotaur::group_block &group, std::ostream &ostream);

	virtual void write(minotaur::content_info &info, minotaur::content_block &content, std::ostream &ostream);

	virtual void write(minotaur::content_info &info, minotaur::code_block &code, std::ostream &ostream);

	virtual void write(minotaur::content_info &info, minotaur::quick_code_block &code, std::ostream &ostream);

	virtual void write(minotaur::content_info &info, minotaur::call_code_block &code, std::ostream &ostream);
public:
	template_builder();

	virtual void write(minotaur::file_info &finfo, std::ostream &ostream);
};
}
#endif // ICARUSFRAMEWORK_MINOTAUR_COMPILER_TEMPLATEBUILDER_H
