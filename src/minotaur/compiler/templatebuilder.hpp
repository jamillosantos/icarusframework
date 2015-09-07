/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 19, 2015
 */

#ifndef ICARUSFRAMEWORK_MINOTAUROS_COMPILER_TEMPLATEBUILDER_H
#define ICARUSFRAMEWORK_MINOTAUROS_COMPILER_TEMPLATEBUILDER_H

#include <iostream>
#include "metainfo.hpp"

namespace minotaur
{
namespace compiler
{
class TemplateBuilder
{
private:
	int documentLevel;
	int classLevel;
	int stringBlockLevel;
	int writeBlockLevel;
public:
	TemplateBuilder()
		: documentLevel(0), classLevel(0), stringBlockLevel(0)
	{ }

	virtual void beginDocument(MetaInfo &fileInfo, std::ostream &ostream)
	{
		this->documentLevel++;
	}

	virtual void endDocument(MetaInfo &fileInfo, std::ostream &ostream)
	{
		this->documentLevel--;
	}

	const int getDocumentLevel()
	{
		return this->documentLevel;
	}

	virtual void beginClass(MetaInfo &fileInfo, std::ostream &ostream)
	{
		this->classLevel++;
	}

	virtual void endClass(MetaInfo &fileInfo, std::ostream &ostream)
	{
		this->classLevel--;
	}

	const int getClassLevel()
	{
		return this->classLevel;
	}

	virtual void beginStringBlock(MetaInfo &fileInfo, std::ostream &ostream)
	{
		this->stringBlockLevel++;
	}

	virtual void endStringBlock(MetaInfo &fileInfo, std::ostream &ostream)
	{
		this->stringBlockLevel--;
	}

	const int getStringBlockLevel()
	{
		return this->stringBlockLevel;
	}

	virtual void beginWriteBlock(MetaInfo &fileInfo, std::ostream &ostream)
	{
		this->writeBlockLevel++;
	}

	virtual void endWriteBlock(MetaInfo &fileInfo, std::ostream &ostream)
	{
		this->writeBlockLevel--;
	}

	const int getWriteBlockLevel()
	{
		return this->writeBlockLevel;
	}

	virtual void writeChar(MetaInfo &fileInfo, std::ostream &ostream, char c)
	{
		ostream << c;
	}
};
}
}
#endif // ICARUSFRAMEWORK_MINOTAUROS_COMPILER_TEMPLATEBUILDER_H
