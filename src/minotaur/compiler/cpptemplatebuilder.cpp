
#include <minotaur/compiler/cpptemplatebuilder.h>

minotaur::compiler::CppTemplateBuilder::CppTemplateBuilder() : TemplateBuilder()
{ }

void minotaur::compiler::CppTemplateBuilder::document_begin(MetaInfo &fileInfo, std::ostream &ostream)
{
	TemplateBuilder::document_begin(fileInfo, ostream);
	ostream
	<< "/**" << std::endl
	<< " * Auto created." << std::endl
	<< " **/" << std::endl << std::endl
	<< "#include <icarus/result.hpp>" << std::endl
	<< "#include <icarus/statuses.hpp>" << std::endl;
}

void minotaur::compiler::CppTemplateBuilder::class_begin(MetaInfo &fileInfo, std::ostream &ostream)
{
	TemplateBuilder::class_begin(fileInfo, ostream);
	ostream << "namespace templates" << std::endl << "{" << std::endl;
	for (std::string package : fileInfo.package)
	{
		ostream << "namespace " << package << std::endl << "{" << std::endl;
	}
	ostream
	<< "\ticarus::result &" << fileInfo.name << "(icarus::result &out";

	if (!fileInfo.parameters.empty())
		ostream << ", " << fileInfo.parameters;

	ostream
	<< ")" << std::endl
	<< "\t{" << std::endl;
}

void minotaur::compiler::CppTemplateBuilder::class_end(MetaInfo &fileInfo, std::ostream &ostream)
{
	TemplateBuilder::class_end(fileInfo, ostream);
	ostream
	<< "\t\treturn out;" << std::endl
	<< "\t}" << std::endl;
	for (std::string package : fileInfo.package)
		ostream << "} // " << package << std::endl;
	ostream << "} // templates" << std::endl;
}

void minotaur::compiler::CppTemplateBuilder::string_block_begin(MetaInfo &fileInfo, std::ostream &ostream)
{
	TemplateBuilder::string_block_begin(fileInfo, ostream);
	ostream
	<< "\t\tout << \"";
}

void minotaur::compiler::CppTemplateBuilder::string_block_end(MetaInfo &fileInfo, std::ostream &ostream)
{
	TemplateBuilder::string_block_end(fileInfo, ostream);
	ostream
	<< "\";" << std::endl;
}

void minotaur::compiler::CppTemplateBuilder::write_block_begin(MetaInfo &fileInfo, std::ostream &ostream)
{
	TemplateBuilder::write_block_begin(fileInfo, ostream);
	ostream << "\t\tout << ";
}

void minotaur::compiler::CppTemplateBuilder::write_block_end(MetaInfo &fileInfo, std::ostream &ostream)
{
	TemplateBuilder::write_block_end(fileInfo, ostream);
	ostream << ";" << std::endl;
}

void minotaur::compiler::CppTemplateBuilder::write(MetaInfo &fileInfo, std::ostream &ostream, char c)
{
	if (this->string_block_level() > 0)
	{
		if (c == '\\')
			ostream << "\\";
		if (c == '"')
			ostream << "\\\"";
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