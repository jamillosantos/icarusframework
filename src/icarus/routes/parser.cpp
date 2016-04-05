/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#include <icarus/routes/parser.h>

#include <icarus/routes/exceptions.h>
#include <icarus/routes/fieldtypes.h>

char icarus::routes::parser::last_char()
{
	return this->inputStreamBuffer[this->currentInputStreamChar - 1];
}

bool icarus::routes::parser::read_char(char *cc)
{
	if (this->_next_char != 0)
	{
		*cc = this->_next_char;
		this->_next_char = 0;
		return  true;
	}

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

void icarus::routes::parser::read_until_non_blank(char *cc)
{
	while (this->read_char(cc))
	{
		if (!((*cc == '\n') || (*cc == '\r') || (*cc == ' ') || (*cc == '\t')))
			return;
	}
	throw icarus::premature_eof();
}

std::string icarus::routes::parser::read_until(std::string chars)
{
	char cc;
	std::string result;
	unsigned int i;
	while (this->read_char(&cc))
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


std::string icarus::routes::parser::read_identifier()
{
	char cc;
	std::string result;
	unsigned int i = 0;
	while (this->read_char(&cc))
	{
		if (
			(
				(cc >= 'A')
				&& (cc <= 'Z')
			)
			|| (
				(cc >= 'a')
				&& (cc <= 'z')
			)
			|| (cc == '_')
			|| (
				(i > 0)
				&& (cc >= '0')
				&& (cc <= '9')
			)
		)
			result += cc;
		else
		{
			this->_next_char = cc;
			return result;
		}
		++i;
	}
	throw icarus::premature_eof();
}

void icarus::routes::parser::run_comment_line()
{
	char cc;
	while (this->read_char(&cc))
	{
		if (cc == '\n')
			break;
	}
}

void icarus::routes::parser::run_comment_multiline()
{
	char cc;
	while (this->read_char(&cc))
	{
		if (cc == '*')
		{
			if (this->read_char(&cc))
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

std::string icarus::routes::parser::run_skip_quotes()
{
	char cc;
	std::stringstream stream;
	while (this->read_char(&cc))
	{
		stream << cc;
		if (cc == '\\')
		{
			if (!this->read_char(&cc))
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

std::string icarus::routes::parser::run_skip_regex_brackets()
{
	unsigned int count = 1;
	char cc;
	std::stringstream stream;
	while (this->read_char(&cc))
	{
		stream << cc;
		if (cc == '\\')
		{
			if (!this->read_char(&cc))
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

void icarus::routes::parser::run_line_method_parameters(call_method &callMethod)
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
				callMethod.add(paramstream.str(), icarus::routes::method_param_type::NORMAL, stream.str());
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
		else if (cc == '@')
		{
			std::string identifier = this->read_identifier();
			if (identifier.empty())
			{
				// TODO Throw an exception
				std::cerr << "Identifier expected.";
				std::exit(99);
			}
			else
				callMethod.add("", icarus::routes::method_param_type::IDENTIFIER, identifier);
			this->read_char(&cc);
		}
		else
		{
			stream << cc;
			if (!this->read_char(&cc))
			{
				throw icarus::premature_eof();
			}
		}
	}
}

char icarus::routes::parser::read_uri(icarus::routes::composed_uri &uri)
{
	std::stringstream stream;
	char cc;
	this->read_until_non_blank(&cc);
	stream << cc;
	while (this->read_char(&cc))
	{
		if (cc == '\\')
		{
			if (this->read_char(&cc))
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
			while (this->read_char(&cc))
			{
				if (cc == ':')
				{
					tmpstr = stream.str();
					stream.str("");

					while (this->read_char(&cc))
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
							if (!this->read_char(&cc))
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

void icarus::routes::parser::run_group(icarus::routes::routes &data, unsigned int level)
{
	unsigned int cl = this->currentLine;
	std::stringstream stream;
	icarus::routes::group *group;
	data.add(group = new icarus::routes::group(cl));
	char cc = this->read_uri(group->uri());
	if (cc != '{')
		this->read_until("{");
	this->run_doc(*group, level + 1);
}

void icarus::routes::parser::run_line(icarus::routes::routes &data, unsigned int level)
{
	icarus::routes::route route(this->currentLine);
	std::stringstream stream;
	char cc = this->last_char();
	stream << cc;
	while (this->read_char(&cc))
	{
		if ((cc == ' ') || (cc == '\t'))
		{
			if (stream.str() == "on")
			{
				this->run_group(data, level);
				return;
			}
			else
			{
				route.http_method(stream.str());
				stream.str("");    // clear stream

				icarus::routes::group *parentGroup = dynamic_cast<icarus::routes::group*>(&data);
				if (parentGroup)
				{
					for (icarus::routes::regex_token &token : parentGroup->uri().tokens())
					{
						route.uri().add(token.name(), token.regex());
					}
				}
				this->read_uri(route.uri());
				this->read_until_non_blank(&cc);
				stream << cc;

				icarus::routes::call_method callMethod;
				while (this->read_char(&cc))
				{
					if (cc == '(')
					{
						callMethod.path(stream.str());
						stream.str("");
						this->run_line_method_parameters(callMethod);
						route.call_method(callMethod);
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

void icarus::routes::parser::run_doc(icarus::routes::routes &data, unsigned int level)
{
	char cc;
	while (this->read_char(&cc))
	{
		// Finds a first char to be a comment.
		if (cc == '/')
		{
			if (this->read_char(&cc))
			{
				// Line comment
				if (cc == '/')
				{
					this->run_comment_line();
				}
					// Multiline comment
				else if (cc == '*')
				{
					this->run_comment_multiline();
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
			this->run_line(data, level);
		}
	}
}

icarus::routes::parser::parser(std::string inputFolder)
	: inputFolder(inputFolder), inputStreamBufferSize(0), currentInputStreamChar(0), currentLine(0),
	  currentChar(0), _next_char(0)
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
			this->run_doc(data, 0);
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
