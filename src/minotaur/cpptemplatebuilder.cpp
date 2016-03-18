
#include <minotaur/cpptemplatebuilder.h>

minotaur::cpp_template_builder::cpp_template_builder()
	: minotaur::template_builder()
{ }

void minotaur::cpp_template_builder::document_begin(minotaur::file_info &fileInfo, std::ostream &ostream)
{
	template_builder::document_begin(fileInfo, ostream);
	ostream
	<< "/**" << std::endl
	<< " * Auto created." << std::endl
	<< " **/" << std::endl << std::endl
	<< "#include <icarus/result.h>" << std::endl
	<< "#include <icarus/statuses.h>" << std::endl;
}

void minotaur::cpp_template_builder::class_begin(minotaur::file_info &fileInfo, std::ostream &ostream)
{
	template_builder::class_begin(fileInfo, ostream);
	ostream << "namespace views" << std::endl << "{" << std::endl;
	for (std::string package : fileInfo.package)
	{
		ostream << "namespace " << package << std::endl << "{" << std::endl;
	}

	ostream << "\ttemplate <class T>" << std::endl;
	ostream << "\tT &" << fileInfo.name << "(";
	ostream << "T &out";
	for (const std::unique_ptr<icarus::routes::method_param> &param : fileInfo.parameters)
	{
		ostream << ", " << param->cpp();
	}

	ostream
		<< ")" << std::endl
		<< "\t{" << std::endl;
}

void minotaur::cpp_template_builder::class_end(minotaur::file_info &fileInfo, std::ostream &ostream)
{
	template_builder::class_end(fileInfo, ostream);
	ostream
	<< "\t\treturn out;" << std::endl
	<< "\t}" << std::endl;
	for (std::string package : fileInfo.package)
		ostream << "} // " << package << std::endl;
	ostream << "} // templates" << std::endl;
}

void minotaur::cpp_template_builder::write(minotaur::content_block &content, std::ostream &ostream)
{
	ostream << "\t\tout << \"";
	this->write(content.content, ostream);
	ostream << "\";" << std::endl;
}

void minotaur::cpp_template_builder::write(minotaur::code_block &code, std::ostream &ostream)
{
	ostream << code.content;
}

void minotaur::cpp_template_builder::write(minotaur::quick_code_block &code, std::ostream &ostream)
{
	ostream << "\t\tout << " << code.content << ";" << std::endl;
}

void minotaur::cpp_template_builder::write(const std::string &data, std::ostream &ostream)
{
	for (auto c = data.begin(); c != data.end(); ++c)
	{
		if (*c == '\\')
			ostream << "\\";
		if (*c == '"')
			ostream << "\\\"";
		else if (*c == '\n')
			ostream << "\\n";
		else if (*c == '\r')
			ostream << "\\n";
		else if (*c == '\t')
			ostream << "\\t";
		else
			ostream << *c;
	}
}

void minotaur::cpp_template_builder::write(minotaur::file_info &finfo, std::ostream &ostream)
{
	template_builder::write(finfo, ostream);
}