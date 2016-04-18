
#include <minotaur/cpptemplatebuilder.h>

minotaur::cpp_template_builder::cpp_template_builder()
	: minotaur::template_builder()
{ }

void minotaur::cpp_template_builder::document_begin(minotaur::file_info &fileInfo, std::ostream &ostream)
{
	template_builder::document_begin(fileInfo, ostream);

	std::string define_name = "VIEWS_";
	unsigned int i = 0;
	for (const std::string &package : fileInfo.package)
	{
		if (++i > 1)
			define_name += "_";
		define_name += package;
	}
	define_name += "_" + fileInfo.name + "_HPP";

	ostream
		<< "/**" << std::endl
		<< " * Auto created." << std::endl
		<< " **/" << std::endl << std::endl
		<< "#ifndef " << define_name << std::endl
		<< "#define " << define_name << std::endl
		<< "#include <icarus/result.h>" << std::endl
		<< "#include <icarus/statuses.h>" << std::endl;
}

void minotaur::cpp_template_builder::document_end(file_info &fileInfo, std::ostream &ostream)
{
	std::string define_name = "VIEWS_";
	unsigned int i = 0;
	for (const std::string &package : fileInfo.package)
	{
		if (++i > 1)
			define_name += "_";
		define_name += package;
	}
	define_name += "_" + fileInfo.name + "_HPP";

	ostream
		<< "#endif // " << define_name << std::endl;

	template_builder::document_end(fileInfo, ostream);
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
	ostream << "} // views" << std::endl;
}

void minotaur::cpp_template_builder::write(minotaur::content_info &info, minotaur::piece *piece, std::ostream &ostream)
{
	minotaur::template_builder::write(info, piece, ostream);
}

void minotaur::cpp_template_builder::write(minotaur::content_info &info, minotaur::group_block &group, std::ostream &ostream)
{
	for (const std::unique_ptr<minotaur::piece> &piece : group.pieces)
		this->write(info, piece.get(), ostream);
}

void minotaur::cpp_template_builder::write(minotaur::content_info &info, minotaur::content_block &content, std::ostream &ostream)
{
	for (unsigned int i = 0; i < info.level + 2; ++i)
		ostream << "\t";
	ostream << info.stream << " << \"";
	this->write(content.content, ostream);
	ostream << "\";" << std::endl;
}

void minotaur::cpp_template_builder::write(minotaur::content_info &info, minotaur::code_block &code, std::ostream &ostream)
{
	ostream << code.content;
}

void minotaur::cpp_template_builder::write(minotaur::content_info &info, minotaur::quick_code_block &code, std::ostream &ostream)
{
	for (unsigned int i = 0; i < info.level + 2; ++i)
		ostream << "\t";
	ostream << info.stream << " << " << code.content << ";" << std::endl;
}

void minotaur::cpp_template_builder::write(minotaur::content_info &info, minotaur::call_code_block &code, std::ostream &ostream)
{
	std::string tab;
	for (unsigned int i = 0; i < info.level + 2; ++i)
		tab += "\t";
	ostream << tab << "{" << std::endl;
	std::stringstream subcall;
	for (unsigned int i = 0; i < code.pieces.size(); i++)
	{
		minotaur::content_info subinfo("content" + std::to_string(i) + "_" + std::to_string(info.level), info.level+1);
		ostream << tab << "\ticarus::content " << subinfo.stream << ";" << std::endl;
		this->write(subinfo, code.pieces[0].get(), ostream);
		subcall << ", " << subinfo.stream;
	}
	ostream << tab << "\t" << code.name << "(" << info.stream << subcall.rdbuf();
	if (!code.parameters.empty())
		ostream << ", " << code.parameters;
	ostream << ");" << std::endl;
	ostream << tab << "}" << std::endl;
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
