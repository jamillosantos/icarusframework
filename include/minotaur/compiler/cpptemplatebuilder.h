/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 19, 2015
 */

#ifndef ICARUSFRAMEWORK_MINOTAUR_COMPILER_CPPTEMPLATEBUILDER_H
#define ICARUSFRAMEWORK_MINOTAUR_COMPILER_CPPTEMPLATEBUILDER_H

#include <minotaur/compiler/templatebuilder.h>
#include <minotaur/compiler/metainfo.h>

namespace minotaur
{
namespace compiler
{
class CppTemplateBuilder
	: public TemplateBuilder
{
public:
	CppTemplateBuilder();

	virtual void document_begin(MetaInfo &fileInfo, std::ostream &ostream);

	virtual void class_begin(MetaInfo &fileInfo, std::ostream &ostream);

	virtual void class_end(MetaInfo &fileInfo, std::ostream &ostream);

	virtual void string_block_begin(MetaInfo &fileInfo, std::ostream &ostream);

	virtual void string_block_end(MetaInfo &fileInfo, std::ostream &ostream);

	virtual void write_block_begin(MetaInfo &fileInfo, std::ostream &ostream);

	virtual void write_block_end(MetaInfo &fileInfo, std::ostream &ostream);

	virtual void write(MetaInfo &fileInfo, std::ostream &ostream, char c);
};
}
}
#endif // ICARUSFRAMEWORK_MINOTAUR_COMPILER_CPPTEMPLATEBUILDER_H
