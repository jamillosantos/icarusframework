/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 19, 2015
 */

#ifndef ICARUSFRAMEWORK_MINOTAUROS_COMPILER_CPPTEMPLATEBUILDER_H
#define ICARUSFRAMEWORK_MINOTAUROS_COMPILER_CPPTEMPLATEBUILDER_H

#include "templatebuilder.hpp"

namespace minotaur
{
namespace compiler
{
class CppTemplateBuilder
	: public TemplateBuilder
{
public:
	CppTemplateBuilder()
		: TemplateBuilder()
	{ }

	virtual void beginDocument(MetaInfo &fileInfo, std::ostream &ostream) override
	{
		TemplateBuilder::beginDocument(fileInfo, ostream);
		ostream
			<< "/**" << std::endl
			<< " * Auto created." << std::endl
			<< " **/" << std::endl << std::endl
			<< "#include <simpleweb/server_http.hpp>" << std::endl
			<< "#include <icarus/http/response.hpp>" << std::endl;
	}

	virtual void beginClass(MetaInfo &fileInfo, std::ostream &ostream) override
	{
		TemplateBuilder::beginClass(fileInfo, ostream);
		ostream << "namespace templates" << std::endl << "{" << std::endl;
		for (std::string package : fileInfo.package)
		{
			ostream << "namespace " << package << std::endl << "{" << std::endl;
		}
		ostream
			<< "\tminotaur::OChainStream " << fileInfo.name << "(";

		if (!fileInfo.parameters.empty())
			ostream << ", " << fileInfo.parameters;

		ostream
			<< ")" << std::endl
			<< "\t{" << std::endl
			<< "\t\tminotaur::OChainStream out;" << std::endl;
	}

	virtual void endClass(MetaInfo &fileInfo, std::ostream &ostream) override
	{
		TemplateBuilder::endClass(fileInfo, ostream);
		ostream
			<< "\t\treturn out;" << std::endl
			<< "\t}" << std::endl;
		for (std::string package : fileInfo.package)
			ostream << "} // " << package << std::endl;
		ostream << "} // templates" << std::endl;
	}

	virtual void beginStringBlock(MetaInfo &fileInfo, std::ostream &ostream) override
	{
		TemplateBuilder::beginStringBlock(fileInfo, ostream);
		ostream
			<< "\t\tout << \"";
	}

	virtual void endStringBlock(MetaInfo &fileInfo, std::ostream &ostream) override
	{
		TemplateBuilder::endStringBlock(fileInfo, ostream);
		ostream
			<< "\";" << std::endl;
	}

	virtual void beginWriteBlock(MetaInfo &fileInfo, std::ostream &ostream) override
	{
		TemplateBuilder::beginWriteBlock(fileInfo, ostream);
		ostream << "\t\tout << ";
	}

	virtual void endWriteBlock(MetaInfo &fileInfo, std::ostream &ostream) override
	{
		TemplateBuilder::endWriteBlock(fileInfo, ostream);
		ostream << ";" << std::endl;
	}

	virtual void writeChar(MetaInfo &fileInfo, std::ostream &ostream, char c) override
	{
		if (this->getStringBlockLevel() > 0)
		{
			if (c == '"')
			{
				ostream << "\\";
				ostream << c;
			}
			else if (c == '\n')
				ostream << "\\n";
			else if (c == '\r')
				ostream << "\\n";
			else if (c == '\t')
				ostream << "\\t";
			else
				ostream << c;
		}
		else
			ostream << c;
	}
};
}
}
#endif // ICARUSFRAMEWORK_MINOTAUROS_COMPILER_CPPTEMPLATEBUILDER_H
