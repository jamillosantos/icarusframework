
#include <minotaur/compiler.h>
#include <minotaur/cpptemplatebuilder.h>

minotaur::compiler::compiler(std::string inputFile, std::string outputFile)
	: input_file(inputFile), output_file(outputFile)
{ }

void minotaur::compiler::compile(template_builder &builder)
{
	boost::filesystem::path id(this->input_file);
	boost::filesystem::path od(this->output_file);

	std::string
		idFullPath = id.string(),
		parentPath;

	std::vector<std::string> currentStack;
	if (boost::filesystem::exists(id))
	{
		if (boost::filesystem::is_directory(id))
		{
			boost::filesystem::recursive_directory_iterator dirEnd;
			for (boost::filesystem::recursive_directory_iterator d(id);
				 d != dirEnd; ++d)
			{
				std::cout << "*" << d->path().string();
				if (boost::filesystem::is_regular(*d))
				{
					std::cout << ". OK";
					std::string extension = d->path().extension().string();
					parentPath = d->path().parent_path().string();

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

					std::string relativePath;
					if (parentPath != idFullPath)
						relativePath = parentPath.substr(idFullPath.length()+1);

					std::ifstream in_stream(fpath.string());
					if (!in_stream)
						throw icarus::open_file("Cannot open input file.");

					minotaur::parse_file c(finfo, in_stream);

					boost::filesystem::path targetCppFile(od.string() + boost::filesystem::path::preferred_separator
						+ (relativePath) + boost::filesystem::path::preferred_separator + c.info.name
						+ c.info.extension + ".hpp"
					);
					boost::filesystem::create_directories(targetCppFile.parent_path().string());

					boost::split(c.info.package, relativePath, boost::is_any_of("\\/"));
					c.parse();

					std::ofstream ostream(targetCppFile.string());
					if (ostream)
					{
						minotaur::cpp_template_builder builder;
						builder.write(finfo, ostream);
					}
					else
						throw icarus::open_file("Cannot create file stream on " + targetCppFile.string() + ".");
				}
				std::cout << std::endl;
			}
		}
		else if (boost::filesystem::is_regular_file(id))
		{
			std::string extension = id.extension().string();
			parentPath = id.parent_path().string();

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

			minotaur::parse_file c(finfo, in_stream);
			c.parse();

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