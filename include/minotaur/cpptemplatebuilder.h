/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 19, 2015
 */

#ifndef ICARUSFRAMEWORK_MINOTAUR_COMPILER_CPPTEMPLATEBUILDER_H
#define ICARUSFRAMEWORK_MINOTAUR_COMPILER_CPPTEMPLATEBUILDER_H

#include <minotaur/templatebuilder.h>
#include <minotaur/metainfo.h>

namespace minotaur
{
class cpp_template_builder
	: public template_builder
{
protected:
	virtual void document_begin(minotaur::file_info &fileInfo, std::ostream &ostream) override;

	virtual void class_begin(minotaur::file_info &fileInfo, std::ostream &ostream) override;

	virtual void class_end(minotaur::file_info &fileInfo, std::ostream &ostream) override;

	virtual void write(minotaur::content_info &info, minotaur::piece *piece, std::ostream &ostream) override;

	virtual void write(minotaur::content_info &info, minotaur::group_block &group, std::ostream &ostream) override;

	virtual void write(minotaur::content_info &info, minotaur::content_block &content, std::ostream &ostream) override;

	virtual void write(minotaur::content_info &info, minotaur::code_block &code, std::ostream &ostream) override;

	virtual void write(minotaur::content_info &info, minotaur::quick_code_block &code, std::ostream &ostream) override;

	virtual void write(minotaur::content_info &info, minotaur::call_code_block &code, std::ostream &ostream) override;

	virtual void write(const std::string &data, std::ostream &ostream);
public:
	cpp_template_builder();

	virtual void write(minotaur::file_info &finfo, std::ostream &ostream) override;
};
}
#endif // ICARUSFRAMEWORK_MINOTAUR_COMPILER_CPPTEMPLATEBUILDER_H
