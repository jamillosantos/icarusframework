/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#include <icarus/routes/parser.h>

#include <icarus/routes/exceptions.h>
#include <icarus/routes/fieldtypes.h>

char icarus::routes::parser::lastChar()
{
	return this->inputStreamBuffer[this->currentInputStreamChar - 1];
}

bool icarus::routes::parser::readChar(char *cc)
{
	if ((this->currentInputStreamChar >= INPUT_STREAM_BUFFER_SIZE) || (this->inputStreamBufferSize == 0))
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

void icarus::routes::parser::readUntilNonBlank(char *cc)
{
	while (this->readChar(cc))
	{
		if (!((*cc == '\n') || (*cc == '\r') || (*cc == ' ') || (*cc == '\t')))
			return;
	}
	throw icarus::premature_eof();
}

std::string icarus::routes::parser::readUntil(std::string chars)
{
	char cc;
	std::string result;
	unsigned int i;
	while (this->readChar(&cc))
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

void icarus::routes::parser::runCommentLine()
{
	char cc;
	while (this->readChar(&cc))
	{
		if (cc == '\n')
			break;
	}
}

void icarus::routes::parser::runCommentMultiLine()
{
	char cc;
	while (this->readChar(&cc))
	{
		if (cc == '*')
		{
			if (this->readChar(&cc))
			{
				if (cc == '/')
				{
					break;
				}
			}
			else
			{
				// Throw an exception
				std::cerr << "Unexpected end of file." << std::endl;
				throw std::exception();
			}
		}
	}
}

std::string icarus::routes::parser::runSkipQuotes()
{
	char cc;
	std::stringstream stream;
	while (this->readChar(&cc))
	{
		stream << cc;
		if (cc == '\\')
		{
			if (!this->readChar(&cc))
			{
				break;
			}
			stream << cc;
		}
		else if (cc == '"')
		{
			stream << cc;
			return stream.str();
		}
	}
	throw icarus::premature_eof();
}

std::string icarus::routes::parser::runSkipRegexBrackets()
{
	unsigned int count = 1;
	char cc;
	std::stringstream stream;
	while (this->readChar(&cc))
	{
		stream << cc;
		if (cc == '\\')
		{
			if (!this->readChar(&cc))
				break;
			stream << cc;
		}
		else if (cc == '(')
		{
			count++;
		}
		else if (cc == ')')
		{
			count--;
			if (count == 0)
			{
				return stream.str();
			}
		}
	}
	throw icarus::premature_eof();
}

void icarus::routes::parser::runLineMethodParameters(call_method &callMethod)
{
	char cc;
	std::stringstream stream, paramstream;
	this->readUntilNonBlank(&cc);
	for (; ;)
	{
		if ((cc == ')') || (cc == ','))
		{
			if ((paramstream.tellp() == 0) && (stream.tellp() == 0))
				break;
			else
			{
				callMethod.add(paramstream.str(), icarus::routes::method_param_type::NORMAL, stream.str());
				paramstream.str("");
				stream.str("");
				if (cc == ')')
					break;
				else
					this->readUntilNonBlank(&cc);
			}
		}
		else if ((cc == ' ') || (cc == '\t') || (cc == '\r') || (cc == '\n'))
		{
			if (paramstream.tellp())
				paramstream << " ";
			paramstream << stream.rdbuf();
			stream.str("");
			this->readUntilNonBlank(&cc);
		}
		else
		{
			stream << cc;
			if (!this->readChar(&cc))
			{
				throw icarus::premature_eof();
			}
		}
	}
}

char icarus::routes::parser::readURI(icarus::routes::composed_uri &uri)
{
	std::stringstream stream;
	char cc;
	this->readUntilNonBlank(&cc);
	stream << cc;
	while (this->readChar(&cc))
	{
		if (cc == '\\')
		{
			if (this->readChar(&cc))
				stream << '\\' << cc;
			else
			{
				throw icarus::premature_eof();
			}
		}
		else if (cc == '<')
		{
			bool success = false;
			std::string tmpstr = stream.str();
			if (!tmpstr.empty())
			{
				uri.add(tmpstr);
				stream.str("");
			}
			while (this->readChar(&cc))
			{
				if (cc == ':')
				{
					tmpstr = stream.str();
					stream.str("");

					while (this->readChar(&cc))
					{
						if (cc == '>')
						{
							uri.add(tmpstr, stream.str());
							stream.str("");
							success = true;
							break;
						}
						else if (cc == '\\')
						{
							stream << cc;
							if (!this->readChar(&cc))
								break;
							stream << cc;
						}
						else
							stream << cc;
					}
				}
				else if (cc == '>')
				{
					uri.add(stream.str(), "");
					stream.str("");
					success = true;
					break;
				}
				else if ((cc == ' ') || (cc == '\t') || (cc == '\r') || (cc == '\n'))
					break;
				else if (boost::regex_match(std::to_string(cc), variablesName))
					stream << cc;
				else
				{
					throw icarus::routes::invalid_Char(this->currentLine, this->currentChar);
				}
			}
			if (!success)
			{
				throw icarus::premature_eof();
			}
		}
		else if ((cc == ' ') || (cc == '\t') || (cc == '\n') || (cc == '{'))
		{
			std::string tmpstr = stream.str();
			if (!tmpstr.empty())
				uri.add(tmpstr);
			return cc;
		}
		else
		{
			stream << cc;
		}
	}
	throw icarus::premature_eof();
}

void icarus::routes::parser::runGroup(icarus::routes::routes &data, unsigned int level)
{
	unsigned int cl = this->currentLine;
	std::stringstream stream;
	icarus::routes::group *group;
	data.add(group = new icarus::routes::group(cl));
	char cc = this->readURI(group->uri());
	if (cc != '{')
		this->readUntil("{");
	this->runDoc(*group, level + 1);
}

void icarus::routes::parser::runLine(icarus::routes::routes &data, unsigned int level)
{
	icarus::routes::route route(this->currentLine);
	std::stringstream stream;
	char cc = this->lastChar();
	stream << cc;
	while (this->readChar(&cc))
	{
		if ((cc == ' ') || (cc == '\t'))
		{
			if (stream.str() == "on")
			{
				this->runGroup(data, level);
				return;
			}
			else
			{
				route.httpMethod(stream.str());
				stream.str("");    // clear stream

				icarus::routes::group *parentGroup = dynamic_cast<icarus::routes::group*>(&data);
				if (parentGroup)
				{
					for (icarus::routes::regex_token &token : parentGroup->uri().tokens())
					{
						route.uri().add(token.name(), token.regex());
					}
				}
				this->readURI(route.uri());
				this->readUntilNonBlank(&cc);
				stream << cc;

				icarus::routes::call_method callMethod;
				while (this->readChar(&cc))
				{
					if (cc == '(')
					{
						callMethod.path(stream.str());
						stream.str("");
						this->runLineMethodParameters(callMethod);
						route.callMethod(callMethod);
						int i = 0;
						for (icarus::routes::regex_token &token : route.uri().tokens())
						{
							if (token.regex().empty())
							{
								for (const icarus::routes::method_param &param : callMethod.params())
								{
									if (param.name() == token.name())
									{
										if (
											(param.type() == "string") ||
											(param.type() == "std::string")
										)
											token.regex("");
										else
										{
											const std::string &regex = icarus::routes::field_types::types.get(param.type());
											if (!regex.empty())
												token.regex(regex);
										}
										break;
									}
								}
							}
							// TODO throw a exception: Data type is not assigned.
							// if (token.regex().empty())
							// 		// throw an exception
							i++;
						}

						data.add(new icarus::routes::route(route));
						return;
					}
					else if (cc == '\n')
					{
						throw icarus::routes::incomplete_declaration(this->currentLine, this->currentChar);
					}
					else
					{
						stream << cc;
					}
				}
			}
		}
		else if (cc == '\n')
		{
			throw icarus::routes::incomplete_declaration(this->currentLine, this->currentChar);
		}
		else
			stream << cc;
	}
	throw icarus::premature_eof();
}

void icarus::routes::parser::runDoc(icarus::routes::routes &data, unsigned int level)
{
	char cc;
	while (this->readChar(&cc))
	{
		// Finds a first char to be a comment.
		if (cc == '/')
		{
			if (this->readChar(&cc))
			{
				// Line comment
				if (cc == '/')
				{
					this->runCommentLine();
				}
					// Multiline comment
				else if (cc == '*')
				{
					this->runCommentMultiLine();
				}
			}
			else
			{
				throw icarus::premature_eof();
			}
		}
		else if (cc == '}')
		{
			if (level > 0)
			{
				return;
			}
			else
			{
				throw new icarus::routes::invalid_Char(this->currentLine, this->currentChar);
			}
		}
		else if ((cc != '\n') && (cc != '\r') && (cc != ' ') && (cc != '\t'))
		{
			this->runLine(data, level);
		}
	}
}

icarus::routes::parser::parser(std::string inputFolder)
	: inputFolder(inputFolder), inputStreamBufferSize(0), currentInputStreamChar(0), currentLine(0),
	  currentChar(0)
{ }

void icarus::routes::parser::parse(std::string inputFile, icarus::routes::document &data)
{
	boost::filesystem::path ifp(inputFile);
	if (boost::filesystem::exists(ifp))
	{
		std::ifstream *s;
		this->istream.reset(s = new std::ifstream());
		std::ios_base::iostate exceptionMask = s->exceptions() | std::ios::failbit;
		s->exceptions(exceptionMask);
		try
		{
			s->open(ifp.string());
			this->runDoc(data, 0);
		}
		catch (std::ios_base::failure &e)
		{
			throw icarus::open_file(
					(boost::locale::format(boost::locale::translate("Cannot open file '{1}'")) % ifp).str(), &e);
		}
	}
	else
	{
		throw icarus::file_not_found(ifp.string());
	}
}

boost::regex icarus::routes::parser::variablesName("[a-zA-Z_0-9]+");
