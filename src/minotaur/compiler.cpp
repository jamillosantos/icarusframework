
#include <minotaur/compiler.h>
#include <minotaur/cpptemplatebuilder.h>

bool minotaur::compiler::needs_update(const boost::filesystem::path &source, const boost::filesystem::path &compiled)
{
	return
		(!boost::filesystem::exists(compiled))
		|| (boost::filesystem::last_write_time(source) > boost::filesystem::last_write_time(compiled));
}

minotaur::compiler::compiler(std::string inputFile, std::string outputFile)
	: input_file(inputFile), output_file(outputFile)
{ }

void minotaur::compiler::compile(template_builder &builder)
{
	boost::filesystem::path id(this->input_file);
	boost::filesystem::path od(this->output_file);

	std::string
		id_full_path = id.string(),
		parent_path;

	std::vector<std::string> current_stack;
	if (boost::filesystem::exists(id))
	{
		if (boost::filesystem::is_directory(id))
		{
			boost::filesystem::recursive_directory_iterator dir_end;
			for (boost::filesystem::recursive_directory_iterator d(id);
				 d != dir_end; ++d)
			{
				std::cout << "*" << d->path().string();
				if (boost::filesystem::is_regular(*d))
				{
					std::string extension = d->path().extension().string();
					parent_path = d->path().parent_path().string();

					minotaur::file_info finfo;
					boost::filesystem::path fpath = (*d).path();

					finfo.extension = fpath.extension().string();
					std::string fname = fpath.filename().string();
					unsigned long pos = fname.find('.');
					if (pos != std::string::npos)
					{
						finfo.name = fname.substr(0, pos);
						finfo.subextension = fname.substr(pos, fname.length() - pos - finfo.extension.size());
					}
					else
					{
						finfo.name = fpath.filename().string();
						finfo.subextension = "";
					}
					finfo.path = fpath.parent_path().string();
					finfo.fullPath = fpath.string();

					std::string relative_path;
					if (parent_path != id_full_path)
						relative_path = parent_path.substr(id_full_path.length()+1);

					boost::filesystem::path target_compiled_file(
						od.string() + boost::filesystem::path::preferred_separator + (relative_path)
						+ boost::filesystem::path::preferred_separator + finfo.name + finfo.extension + ".hpp"
					);

					if (this->needs_update(fpath, target_compiled_file))
					{
						std::ifstream in_stream(fpath.string());
						if (!in_stream)
							throw icarus::open_file("Cannot open input file.");

						minotaur::parser_file c(in_stream);

						boost::filesystem::create_directories(target_compiled_file.parent_path().string());

						boost::split(finfo.package, relative_path, boost::is_any_of("\\/"));
						c.parse(finfo);

						std::ofstream ostream(target_compiled_file.string());
						if (ostream)
						{
							minotaur::cpp_template_builder builder;
							builder.write(finfo, ostream);
						}
						else
							throw icarus::open_file("Cannot create file stream on " + target_compiled_file.string() + ".");
						std::cout << ". OK";
					}
					else
					{
						std::cout << ". Skipped";
					}
				}
				std::cout << std::endl;
			}
		}
		else if (boost::filesystem::is_regular_file(id))
		{
			std::string extension = id.extension().string();
			parent_path = id.parent_path().string();

			std::ifstream in_stream(id.string());
			if (!in_stream)
				throw icarus::open_file("Cannot open input file.");

			minotaur::file_info finfo;

			finfo.extension = id.extension().string();
			std::string fname = id.filename().string();
			unsigned long pos = fname.find('.');
			if (pos != std::string::npos)
			{
				finfo.name = fname.substr(0, pos);
				finfo.subextension = fname.substr(pos, fname.length() - pos - finfo.extension.size());
			}
			else
			{
				finfo.name = id.filename().string();
				finfo.subextension = "";
			}
			finfo.path = id.parent_path().string();
			finfo.fullPath = id.string();

			minotaur::parser_file c(in_stream);
			c.parse(finfo);

			if (this->needs_update(id, od))
			{
				std::ofstream ostream(od.string());
				if (ostream)
				{
					minotaur::cpp_template_builder builder;
					builder.write(finfo, ostream);
				}
				else
				{
					// TODO: specialize exception
					std::cerr << "Cannot create file stream on " << od.string() << "." << std::endl;
					throw std::exception();
				}
			}
		}
		else
		{
			// TODO: Throw an exception.
		}
	}
	else
	{
		// TODO: Throw an exception.
	}
}
