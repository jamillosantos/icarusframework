/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 19, 2015
 */

#ifndef ICARUSFRAMEWORK_MINOTAUR_COMPILER_TEMPLATEBUILDER_H
#define ICARUSFRAMEWORK_MINOTAUR_COMPILER_TEMPLATEBUILDER_H

#include <iostream>

#include <minotaur/compiler/metainfo.h>

namespace minotaur
{
namespace compiler
{
class TemplateBuilder
{
private:
	int _document_level;
	int _class_level;
	int _string_block_level;
	int _write_block_level;
public:
	TemplateBuilder();

	virtual void document_begin(MetaInfo &fileInfo, std::ostream &ostream);

	virtual void document_end(MetaInfo &fileInfo, std::ostream &ostream);

	const int document_level();

	virtual void class_begin(MetaInfo &fileInfo, std::ostream &ostream);

	virtual void class_end(MetaInfo &fileInfo, std::ostream &ostream);

	const int class_level();

	virtual void string_block_begin(MetaInfo &fileInfo, std::ostream &ostream);

	virtual void string_block_end(MetaInfo &fileInfo, std::ostream &ostream);

	const int string_block_level();

	virtual void write_block_begin(MetaInfo &fileInfo, std::ostream &ostream);

	virtual void write_block_end(MetaInfo &fileInfo, std::ostream &ostream);

	const int write_block_level();

	virtual void write(MetaInfo &fileInfo, std::ostream &ostream, char c);
};
}
}
#endif // ICARUSFRAMEWORK_MINOTAUR_COMPILER_TEMPLATEBUILDER_H
