/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 17, 2015
 */

#ifndef ICARUSFRAMEWORK_MINOTAUROS_COMPILER_COMPILEFILE_HPP
#define ICARUSFRAMEWORK_MINOTAUROS_COMPILER_COMPILEFILE_HPP

#include <vector>
#include <string>
#include <memory>

#include <boost/filesystem.hpp>
#include <fstream>
#include <sstream>
#include <boost/regex.hpp>

#include "exceptions.hpp"
#include "metainfo.hpp"
#include "templatebuilder.hpp"

#define INPUT_STREAM_BUFFER_SIZE 4096

namespace minotaur
{
namespace compiler
{
class CompileFile
{
public:
	static const boost::regex variableChars;

	MetaInfo info;
private:

	boost::filesystem::path file;

	TemplateBuilder &templateBuilder;

	size_t currentLine;
	size_t currentChar;

	std::unique_ptr<std::ifstream> istream;
	std::ostream &ostream;

	char inputStreamBuffer[INPUT_STREAM_BUFFER_SIZE];
	size_t inputStreamBufferSize;
	size_t currentInputStreamChar;

	bool initialized;

	char lastChar()
	{
		return this->inputStreamBuffer[this->currentInputStreamChar - 1];
	}

	bool readChar(char *cc)
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

	void runParameters()
	{
		char cc;
		std::stringstream str;
		while (this->readChar(&cc))
		{
			if (cc == ')')
			{
				break;
			}
			else
			{
				str << cc;
			}
		}
		this->info.parameters = str.str();
	}

	void runQuotes()
	{
		char cc;
		while (this->readChar(&cc))
		{
			this->templateBuilder.writeChar(this->info, this->ostream, cc);
			if (cc == '\\')
			{
				if (this->readChar(&cc))
				{
					this->templateBuilder.writeChar(this->info, this->ostream, cc);
				}
				else
				{
					// TODO: Throw an exception.
					std::cerr << "Cannot read anymore." << std::endl;
				}
			}
			else if (cc == '"')
			{
				break;
			}
		}
	}

	void runBrackets()
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
					this->templateBuilder.writeChar(this->info, this->ostream, cc);
				}
			}
			else
			{
				this->templateBuilder.writeChar(this->info, this->ostream, cc);
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

	void runQuickEchoBrackets()
	{
		char cc;
		unsigned int bracketCount = 1;
		while (this->readChar(&cc))
		{
			if (cc == ')')
			{
				if (--bracketCount == 0)
				{
					this->templateBuilder.writeChar(this->info, this->ostream, cc);
					break;
				}
				else
				{
					this->templateBuilder.writeChar(this->info, this->ostream, cc);
				}
			}
			else
			{
				this->templateBuilder.writeChar(this->info, this->ostream, cc);
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

	void runQuickEcho()
	{
		this->templateBuilder.beginWriteBlock(this->info, this->ostream);
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
						this->ostream << "::";
					}
					else
					{
						// TODO: Throw an exception
						std::cerr << "runQuickEcho(): Invalid char at (" << this->currentLine << ":" << this->currentChar << ") " << std::endl;
						return;
					}
				}
				else
				{
					// TODO: Throw an exception.
					std::cerr << "runQuickEcho(): Cannot read char at (" << this->currentLine << ":" << this->currentChar << ") " << std::endl;
					return;
				}
			}
			else if (cc[0] == '(')
			{
				this->templateBuilder.writeChar(this->info, this->ostream, cc[0]);;
				this->runQuickEchoBrackets();
			}
			else if (cc[0] == ';')
			{
				break;
			}
			else
			{
				this->templateBuilder.writeChar(this->info, this->ostream, cc[0]);;
			}
			count++;
		}
		while (this->readChar(&cc[0]));

		this->templateBuilder.endWriteBlock(this->info, this->ostream);
	}

	void writeChar(char cc)
	{
		if (!initialized)
		{
			initialized = true;

			this->templateBuilder.beginDocument(this->info, this->ostream);
			this->templateBuilder.beginClass(this->info, this->ostream);
		}

		if (this->templateBuilder.getStringBlockLevel() == 0)
			this->templateBuilder.beginStringBlock(this->info, this->ostream);
		this->templateBuilder.writeChar(this->info, this->ostream, cc);
	}

public:
	CompileFile(boost::filesystem::path file, std::ostream &ostream, TemplateBuilder &templateBuilder)
		: file(file), templateBuilder(templateBuilder), currentLine(0), currentChar(0), ostream(ostream),
		  currentInputStreamChar(INPUT_STREAM_BUFFER_SIZE), initialized(false)
	{ }


	void compile()
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
							if (this->templateBuilder.getStringBlockLevel() > 0)
								this->templateBuilder.endStringBlock(this->info, this->ostream);

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
							this->templateBuilder.writeChar(this->info, this->ostream, cc);
						}
						else
						{
							if (this->templateBuilder.getStringBlockLevel() > 0)
								this->templateBuilder.endStringBlock(this->info, this->ostream);
							this->runQuickEcho();
						}
					}
					else
					{
						// TODO: Throw an exception: Cannot read char anymore.
						std::cerr << "Cannot read char anymore." << std::endl;
						return;
					}
				}
				else
				{
					this->writeChar(cc);
				}

				if (!initialized)
				{
					initialized = true;

					this->templateBuilder.beginDocument(this->info, this->ostream);
					this->templateBuilder.beginClass(this->info, this->ostream);
				}
			}

			if (this->templateBuilder.getStringBlockLevel() > 0)
				this->templateBuilder.endStringBlock(this->info, this->ostream);

			this->templateBuilder.endClass(this->info, this->ostream);
			this->templateBuilder.endDocument(this->info, this->ostream);
		}
		else
		{
			// TODO: Throw an exception
			std::cerr << "Cannot open output file." << std::endl;
			return;
		}
	}
};

const boost::regex CompileFile::variableChars("[a-zA-Z0-9_]");
}
}

#endif // ICARUSFRAMEWORK_MINOTAUROS_COMPILER_COMPILEFILE_HPP
