
#include <minotaur/compiler/compilefile.h>

const boost::regex minotaur::compiler::compile_file::variableChars("[a-zA-Z0-9_]");

char minotaur::compiler::compile_file::lastChar()
{
	return this->inputStreamBuffer[this->currentInputStreamChar - 1];
}

bool minotaur::compiler::compile_file::readChar(char *cc)
{
	if (this->currentInputStreamChar >= INPUT_STREAM_BUFFER_SIZE)
	{
		this->inputStreamBufferSize = this->istream->readsome(this->inputStreamBuffer, INPUT_STREAM_BUFFER_SIZE);
		this->currentInputStreamChar = 0;
	}
	else if (this->currentInputStreamChar >= this->inputStreamBufferSize)
	{
		this->inputStreamBuffer[this->currentInputStreamChar - 1] = '\0';
		return false;
	}

	*cc = this->inputStreamBuffer[this->currentInputStreamChar++];
	if (*cc == '\n')
	{
		this->currentLine++;
		this->currentChar = 0;
	}
	else
		this->currentChar++;

	return true;
}

void minotaur::compiler::compile_file::runParameters()
{
	char cc;
	std::stringstream str;
	while (this->readChar(&cc))
	{
		if (cc == ')')
		{
			while (this->readChar(&cc))
			{
				if ((cc != '\n') || (cc != '\r'))
				{
					str << cc;
					break;
				}
			}
			break;
		}
		else
		{
			str << cc;
		}
	}
	this->info.parameters = str.str();
}

void minotaur::compiler::compile_file::runQuotes()
{
	char cc;
	while (this->readChar(&cc))
	{
		this->templateBuilder.write(this->info, this->outputStream, cc);
		if (cc == '\\')
		{
			if (this->readChar(&cc))
				this->templateBuilder.write(this->info, this->outputStream, cc);
			else
				throw icarus::premature_eof();
		}
		else if (cc == '"')
			break;
	}
}

void minotaur::compiler::compile_file::runBrackets()
{
	char cc;
	unsigned int bracketCount = 1;
	while (this->readChar(&cc))
	{
		if (cc == '}')
		{
			if (--bracketCount == 0)
			{
				break;
			}
			else
			{
				this->templateBuilder.write(this->info, this->outputStream, cc);
			}
		}
		else
		{
			this->templateBuilder.write(this->info, this->outputStream, cc);
			if (cc == '"')
			{
				this->runQuotes();
			}
			else if (cc == '{')
			{
				bracketCount++;
			}
		}
	}

	if (bracketCount > 0)
	{
		// TODO: Throw an exception
		std::cerr << "Close bracket not found." << std::endl;
	}
}

void minotaur::compiler::compile_file::runQuickEchoBrackets()
{
	char cc;
	unsigned int bracketCount = 1;
	while (this->readChar(&cc))
	{
		if (cc == ')')
		{
			if (--bracketCount == 0)
			{
				this->templateBuilder.write(this->info, this->outputStream, cc);
				break;
			}
			else
			{
				this->templateBuilder.write(this->info, this->outputStream, cc);
			}
		}
		else
		{
			this->templateBuilder.write(this->info, this->outputStream, cc);
			if (cc == '"')
			{
				this->runQuotes();
			}
			else if (cc == '(')
			{
				bracketCount++;
			}
		}
	}

	if (bracketCount > 0)
	{
		// TODO: Throw an exception
		std::cerr << "Close bracket not found." << std::endl;
	}
}

void minotaur::compiler::compile_file::runQuickEcho()
{
	this->templateBuilder.write_block_begin(this->info, this->outputStream);
	char cc[2] = {this->lastChar(), 0};
	unsigned int count = 0;
	do
	{
		if ((count > 0) && (cc[0] == ':'))
		{
			if (this->readChar(&cc[0]))
			{
				if (cc[0] == ':')
				{
					this->outputStream << "::";
				}
				else
				{
					// TODO: Throw an exception
					std::cerr << "runQuickEcho(): Invalid char at (" << this->currentLine << ":" << this->currentChar << ") " << std::endl;
					return;
				}
			}
			else
				throw icarus::premature_eof(this->currentLine);
		}
		else if (cc[0] == '(')
		{
			this->templateBuilder.write(this->info, this->outputStream, cc[0]);
			this->runQuickEchoBrackets();
		}
		else if (
			(
				(cc[0] >= '0')
				&& (cc[0] <= '9')
			) || (
				(cc[0] >= 'A')
				&& (cc[0] <= 'Z')
			) || (
				(cc[0] >= 'a')
				&& (cc[0] <= 'z')
			)
			)
		{
			this->templateBuilder.write(this->info, this->outputStream, cc[0]);;
		}
		else if (cc[0] == ';')
		{
			this->templateBuilder.write_block_end(this->info, this->outputStream);
			break;
		}
		else
		{
			this->templateBuilder.write_block_end(this->info, this->outputStream);
			this->writeChar(cc[0]);
			break;
		}
		count++;
	}
	while (this->readChar(&cc[0]));
}

void minotaur::compiler::compile_file::writeChar(char cc)
{
	if (!initialized)
	{
		initialized = true;

		this->templateBuilder.document_begin(this->info, this->outputStream);
		this->templateBuilder.class_begin(this->info, this->outputStream);
	}

	if (this->templateBuilder.string_block_level() == 0)
		this->templateBuilder.string_block_begin(this->info, this->outputStream);
	this->templateBuilder.write(this->info, this->outputStream, cc);
}

minotaur::compiler::compile_file::compile_file(boost::filesystem::path file, std::ostream &ostream, TemplateBuilder &templateBuilder)
	: file(file), templateBuilder(templateBuilder), currentLine(0), currentChar(0), outputStream(ostream),
	  currentInputStreamChar(INPUT_STREAM_BUFFER_SIZE), initialized(false)
{
	this->info.extension = this->file.extension().string();
	std::string fname = this->file.filename().string();
	unsigned long pos = fname.find('.');
	if (pos != std::string::npos)
	{
		this->info.name = fname.substr(0, pos);
		this->info.subextension = fname.substr(pos, fname.length() - pos - this->info.extension.size());
	}
	else
	{
		this->info.name = this->file.filename().string();
		this->info.subextension = "";
	}
	this->info.path = this->file.parent_path().string();

	this->info.fullPath = this->file.string();
}

void minotaur::compiler::compile_file::compile()
{
	this->istream.reset(new std::ifstream(this->info.fullPath, std::ifstream::in));
	if (this->istream->is_open())
	{
		char cc;
		this->initialized = false;

		while (this->readChar(&cc))
		{
			if (cc == '@')
			{
				if (this->readChar(&cc))
				{
					if (cc == '{')
					{
						if (this->templateBuilder.string_block_level() > 0)
							this->templateBuilder.string_block_end(this->info, this->outputStream);

						this->runBrackets();
					}
					else if (cc == '(')
					{
						if (initialized)
						{
							// TODO: Throw exception
							std::cerr << "Parameter definitions out of context." << std::endl;
							return;
						}
						else
							this->runParameters();
					}
					else if (cc == '@')
					{
						this->templateBuilder.write(this->info, this->outputStream, cc);
					}
					else
					{
						if (this->templateBuilder.string_block_level() > 0)
							this->templateBuilder.string_block_end(this->info, this->outputStream);
						this->runQuickEcho();
					}
				}
				else
					throw icarus::premature_eof(this->currentLine);
			}
			else
			{
				this->writeChar(cc);
			}

			if (!initialized)
			{
				initialized = true;

				this->templateBuilder.document_begin(this->info, this->outputStream);
				this->templateBuilder.class_begin(this->info, this->outputStream);
			}
		}

		if (this->templateBuilder.string_block_level() > 0)
			this->templateBuilder.string_block_end(this->info, this->outputStream);

		this->templateBuilder.class_end(this->info, this->outputStream);
		this->templateBuilder.document_end(this->info, this->outputStream);
	}
	else
		throw icarus::open_file("Cannot open output file.");
}