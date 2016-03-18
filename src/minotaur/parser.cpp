
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

std::string minotaur::parse_file::read_until_non_blank(char *cc)
{
	std::stringstream block;
	while (this->read(cc))
	{
		if (!((*cc == '\n') || (*cc == '\r') || (*cc == ' ') || (*cc == '\t')))
			return block.str();
		block << cc;
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

void minotaur::parse_file::run_parameters(minotaur::file_info &info)
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
				info.add_param(new icarus::routes::method_param(paramstream.str(), icarus::routes::method_param_type::NORMAL, stream.str()));
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

void minotaur::parse_file::run_content(minotaur::group_block &block, bool skip_at)
{
	char cc;
	std::stringstream sblock;
	while (this->read(&cc))
	{
		if ((cc == '@') || skip_at)
		{
			skip_at = false;
			if (sblock.tellp())
			{
				block.add_piece(new minotaur::content_block(sblock.str()));
				sblock.str("");
			}
			if (this->read(&cc))
			{
				if (cc == '{')
					this->run_block_code(block);
				else if (cc == '(')
					throw minotaur::compiler_exception(this->currentLine, "Parameter definitions out of context.");
				else if (cc == '@')
					sblock << cc;
				else
				{
					this->run_quick_echo(block);
					if (this->last_char() != ';')
					{
						this->read_next = this->last_char();

						std::string tmp = this->read_until_non_blank(&cc);
						if (cc == '{')
						{
							if (dynamic_cast<minotaur::call_code_block *>((block.pieces.rbegin())->get()))
							{
								minotaur::group_block *subblock = new minotaur::group_block();
								this->run_content(*subblock);
								dynamic_cast<minotaur::call_code_block *>((block.pieces.rbegin())->get())->add_piece(subblock);
							}
							else
								throw minotaur::compiler_exception(this->currentLine, "Invalid block call.");
						}
						else
						{
							sblock << tmp;
							this->read_next = cc;
						}
					}
				}
				this->initialized = true;
			}
			else
				throw icarus::premature_eof(this->currentLine);
		}
		else if (cc == '}')
			break;
		else
			sblock << cc;
	}
	if (sblock.tellp())
		block.add_piece(new minotaur::content_block(sblock.str()));
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

void minotaur::parse_file::run_quick_echo_brackets(std::stringstream &sblock)
{
	char cc;
	unsigned int bcount = 1;
	while (this->read(&cc))
	{
		if (cc == ')')
		{
			if (--bcount == 0)
			{
				this->read(&cc);
				break;
			}
		}
		else
		{
			sblock << cc;
			if (cc == '"')
				this->run_quotes(sblock);
			else if (cc == '(')
				bcount++;
		}
	}

	if (bcount > 0)
		throw minotaur::compiler_exception(this->currentLine, "Close bracket not found.");
}

void minotaur::parse_file::run_quick_echo(minotaur::group_block &block)
{
	std::stringstream sblock;
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
					sblock << "::";
				else
					throw minotaur::compiler_exception(this->currentLine, "runQuickEcho(): Invalid char.");
			}
			else
				throw icarus::premature_eof(this->currentLine);
		}
		else if (cc == '(')
		{
			call = true;
			std::stringstream params;
			this->run_quick_echo_brackets(params);
			block.add_piece(new minotaur::call_code_block(sblock.str(), params.str()));
			return;
		}
		else if (
			((cc>= '0') && (cc <= '9'))
			|| ((cc >= 'A') && (cc<= 'Z'))
			|| ((cc >= 'a') && (cc<= 'z'))
		)
			sblock << cc;
		else
			break;
		count++;
	}
	while (this->read(&cc));
	if (!call)
		block.add_piece(new minotaur::quick_code_block(sblock.str()));
}

minotaur::parse_file::parse_file(std::istream &in_stream)
	: istream(in_stream), currentLine(0), currentChar(0), currentInputStreamChar(INPUT_STREAM_BUFFER_SIZE), initialized(false), read_next(0)
{ }

void minotaur::parse_file::parse(minotaur::file_info &info)
{
	char cc;
	this->initialized = false;

	this->read_until_non_blank(&cc);
	do
	{
		if (cc == '@')
		{
			if (this->read(&cc))
			{
				if (cc == '(')
				{
					if (this->initialized)
						throw minotaur::compiler_exception(this->currentLine, "Parameter definitions out of context.");
					else
						this->run_parameters(info);
				}
				else
				{
					this->run_content(info, true);
				}
				this->initialized = true;
			}
			else
				throw icarus::premature_eof(this->currentLine);
		}
		else
			this->run_content(info);
	}
	while (this->read(&cc));
}

void minotaur::parse_file::run_block_code(minotaur::group_block &block)
{
	std::stringstream stringblock;
	this->run_brackets(stringblock);
	block.add_piece(new minotaur::code_block(stringblock.str()));
}
