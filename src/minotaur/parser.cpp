
#include <minotaur/parser.h>

char minotaur::parse_file::last_char()
{
	return this->inputStreamBuffer[this->currentInputStreamChar - 1];
}

bool minotaur::parse_file::read(char *cc)
{
	if (this->read_next != 0)
	{
		*cc = read_next;
		read_next = 0;
		return true;
	}
	if (this->currentInputStreamChar >= INPUT_STREAM_BUFFER_SIZE)
	{
		this->inputStreamBufferSize = this->istream.readsome(this->inputStreamBuffer, INPUT_STREAM_BUFFER_SIZE);
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

void minotaur::parse_file::read_until_non_blank(char *cc)
{
	while (this->read(cc))
	{
		if (!((*cc == '\n') || (*cc == '\r') || (*cc == ' ') || (*cc == '\t')))
			return;
	}
	throw icarus::premature_eof();
}

std::string minotaur::parse_file::read_until(std::string chars)
{
	char cc;
	std::string result;
	unsigned int i;
	while (this->read(&cc))
	{
		for (i = 0; i < chars.size(); i++)
		{
			if (chars[i] == cc)
				return result;
		}
		result += cc;
	}
	throw icarus::premature_eof();
}

void minotaur::parse_file::run_parameters()
{
	char cc;
	std::stringstream stream, paramstream;
	this->read_until_non_blank(&cc);
	for (; ;)
	{
		if ((cc == ')') || (cc == ','))
		{
			if ((paramstream.tellp() == 0) && (stream.tellp() == 0))
				break;
			else
			{
				this->info.add(new icarus::routes::method_param(paramstream.str(), icarus::routes::method_param_type::NORMAL, stream.str()));
				paramstream.str("");
				stream.str("");
				if (cc == ')')
					break;
				else
					this->read_until_non_blank(&cc);
			}
		}
		else if ((cc == ' ') || (cc == '\t') || (cc == '\r') || (cc == '\n'))
		{
			if (paramstream.tellp())
				paramstream << " ";
			paramstream << stream.rdbuf();
			stream.str("");
			this->read_until_non_blank(&cc);
		}
		else
		{
			stream << cc;
			if (!this->read(&cc))
				throw icarus::premature_eof();
		}
	}
}

void minotaur::parse_file::run_quotes(std::stringstream &block)
{
	char cc;
	while (this->read(&cc))
	{
		block << cc;
		if (cc == '\\')
		{
			if (this->read(&cc))
				block << cc;
			else
				throw icarus::premature_eof();
		}
		else if (cc == '"')
			break;
	}
}

void minotaur::parse_file::run_single_quotes(std::stringstream &block)
{
	char cc;
	while (this->read(&cc))
	{
		block << cc;
		if (cc == '\\')
		{
			if (this->read(&cc))
				block << cc;
			else
				throw icarus::premature_eof();
		}
		else if (cc == '\'')
			break;
	}
}

void minotaur::parse_file::run_brackets(std::stringstream &block)
{
	char cc;
	unsigned int bcount = 1;
	while (this->read(&cc))
	{
		if (cc == '}')
		{
			if (--bcount == 0)
				break;
			else
				block << cc;
		}
		else
		{
			block << cc;
			if (cc == '"')
				this->run_quotes(block);
			else if (cc == '"')
				this->run_single_quotes(block);
			else if (cc == '{')
				bcount++;
		}
	}

	if (bcount > 0)
		throw minotaur::compiler_exception(this->currentLine, "Close bracket not found.");
}

void minotaur::parse_file::run_quick_echo_brackets(std::stringstream &block)
{
	char cc;
	unsigned int bcount = 1;
	while (this->read(&cc))
	{
		if (cc == ')')
		{
			block << cc;
			if (--bcount == 0)
				break;
		}
		else
		{
			block << cc;
			if (cc == '"')
				this->run_quotes(block);
			else if (cc == '(')
				bcount++;
		}
	}

	if (bcount > 0)
		throw minotaur::compiler_exception(this->currentLine, "Close bracket not found.");
}

void minotaur::parse_file::run_quick_echo()
{
	std::stringstream block;
	char cc = this->last_char();
	unsigned int count = 0;
	bool call = false;
	do
	{
		if ((count > 0) && (cc == ':'))
		{
			if (this->read(&cc))
			{
				if (cc == ':')
					block << "::";
				else
					throw minotaur::compiler_exception(this->currentLine, "runQuickEcho(): Invalid char.");
			}
			else
				throw icarus::premature_eof(this->currentLine);
		}
		else if (cc == '(')
		{
			call = true;
			block << cc;
			this->run_quick_echo_brackets(block);
		}
		else if (
			((cc>= '0') && (cc <= '9'))
			|| ((cc >= 'A') && (cc<= 'Z'))
			|| ((cc >= 'a') && (cc<= 'z'))
		)
			block << cc;
		else
			break;
		count++;
	}
	while (this->read(&cc));
	if (call)
		this->info.add(new minotaur::code_block(block.str()));
	else
		this->info.add(new minotaur::quick_code_block(block.str()));
}

minotaur::parse_file::parse_file(minotaur::file_info &file, std::istream &in_stream)
	: info(file), currentLine(0), currentChar(0), istream(in_stream),
	  currentInputStreamChar(INPUT_STREAM_BUFFER_SIZE), initialized(false), read_next(0)
{ }

void minotaur::parse_file::parse()
{
	char cc;
	this->initialized = false;

	std::stringstream sblock(std::stringstream::in | std::stringstream::out | std::stringstream::binary);

	if (this->read(&cc))
	{
		do
		{
			if (cc == '@')
			{
				if (sblock.tellp())
				{
					this->info.add(new minotaur::content_block(sblock.str()));
					sblock.str("");
				}
				if (this->read(&cc))
				{
					if (cc == '{')
						this->run_block_code();
					else if (cc == '(')
					{
						if (this->initialized)
							throw minotaur::compiler_exception(this->currentLine,
															   "Parameter definitions out of context.");
						else
							this->run_parameters();
					}
					else if (cc == '@')
						sblock << cc;
					else
					{
						this->run_quick_echo();
						if (this->last_char() != ';')
						{
							this->read_next = this->last_char();
							continue;
						}
					}
					this->initialized = true;
				}
				else
					throw icarus::premature_eof(this->currentLine);
			}
			else
				sblock << cc;

			if (!this->initialized)
				this->initialized = true;
		}
		while (this->read(&cc));

		if (sblock.tellp())
			this->info.add(new minotaur::content_block(sblock.str()));
	}
}

void minotaur::parse_file::run_block_code()
{
	std::stringstream block;
	this->run_brackets(block);
	this->info.add(new minotaur::code_block(block.str()));
}
