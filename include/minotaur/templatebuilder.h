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
class template_builder
{
protected:
	virtual void document_begin(file_info &fileInfo, std::ostream &ostream);

	virtual void document_end(file_info &fileInfo, std::ostream &ostream);

	virtual void class_begin(file_info &fileInfo, std::ostream &ostream);

	virtual void class_end(file_info &fileInfo, std::ostream &ostream);

	virtual void write(minotaur::content_block &content, std::ostream &ostream);

	virtual void write(minotaur::code_block &code, std::ostream &ostream);

	virtual void write(minotaur::quick_code_block &code, std::ostream &ostream);
public:
	template_builder();

	virtual void write(minotaur::file_info &finfo, std::ostream &ostream);
};
}
#endif // ICARUSFRAMEWORK_MINOTAUR_COMPILER_TEMPLATEBUILDER_H
