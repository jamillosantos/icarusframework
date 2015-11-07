/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#ifndef ICARUSFRAMEWORK_ROUTES_PARSER_HPP
#define ICARUSFRAMEWORK_ROUTES_PARSER_HPP

#define INPUT_STREAM_BUFFER_SIZE 4096

#include <string>
#include <boost/filesystem.hpp>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include "data.hpp"
#include "../exceptions.hpp"
#include "exceptions.hpp"

#include <boost/locale.hpp>

#include <boost/regex.hpp>

namespace icarus
{
namespace routes
{
class Parser
{
private:
	static boost::regex variablesName;

	std::unique_ptr<std::istream> istream;

	std::string inputFolder;

	char inputStreamBuffer[INPUT_STREAM_BUFFER_SIZE];
	size_t inputStreamBufferSize;
	size_t currentInputStreamChar;

	size_t currentLine;
	size_t currentChar;

	char lastChar()
	{
		return this->inputStreamBuffer[this->currentInputStreamChar - 1];
	}

	bool readChar(char *cc)
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

	void readUntilNonBlank(char *cc)
	{
		while (this->readChar(cc))
		{
			if (!((*cc == '\n') || (*cc == '\r') || (*cc == ' ') || (*cc == '\t')))
				return;
		}
		throw exceptions::PrematureEOF();
	}

	std::string readUntil(std::string chars)
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
		throw exceptions::PrematureEOF();
	}

	/**
	 * Reads bytes untilfind the end of the comment.
	 */
	void runCommentLine()
	{
		char cc;
		while (this->readChar(&cc))
		{
			if (cc == '\n')
				break;
		}
	}

	/**
	 * Reads bytes until find the end of the comment.
	 */
	void runCommentMultiLine()
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

	std::string runSkipQuotes()
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
		throw exceptions::PrematureEOF();
	}

	std::string runSkipRegexBrackets()
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
		throw exceptions::PrematureEOF();
	}

	void runLineMethodParameters(CallMethod &callMethod)
	{
		char cc;
		std::stringstream stream, paramstream;
		this->readUntilNonBlank(&cc);
		for (;;)
		{
			if ((cc == ')') || (cc == ','))
			{
				if ((paramstream.tellp() == 0) && (stream.tellp() == 0))
					break;
				else
				{
					callMethod.add(paramstream.str(), MethodParam::MethodType::NORMAL, stream.str());
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
					throw exceptions::PrematureEOF();
				}
			}
		}
	}

	void runGroup(Routes &data, unsigned int level)
	{
		unsigned int cl = this->currentLine;
		std::stringstream stream;
		std::string uri = this->readUntil("\n{ ");
		if (uri.empty())
		{
			throw exceptions::routes::InvalidURI(uri);
		}
		else
		{
			Group *group;
			data.add(group = new Group(uri, cl));
			if (this->currentChar != '{')
				this->readUntil("{");
			this->runDoc(*group, level+1);
		}
	}

	void runLine(Routes &data, unsigned int level)
	{
		Route route(this->currentLine);
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
								throw exceptions::PrematureEOF();
							}
						}
						else if (cc == '<')
						{
							bool success = false;
							std::string tmpstr = stream.str();
							if (!tmpstr.empty())
							{
								route.add(tmpstr);
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
											route.add(tmpstr, stream.str());
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
									route.add(stream.str(), "");
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
									throw exceptions::routes::InvalidChar(this->currentLine, this->currentChar);
								}
							}
							if (!success)
							{
								throw exceptions::PrematureEOF();
							}
						}
						else if ((cc == ' ') || (cc == '\t'))
						{
							std::string tmpstr = stream.str();
							if (!tmpstr.empty())
								route.add(tmpstr);
							stream.str("");

							this->readUntilNonBlank(&cc);
							stream << cc;
							CallMethod callMethod;
							while (this->readChar(&cc))
							{
								if (cc == '(')
								{
									callMethod.path(stream.str());
									stream.str("");
									this->runLineMethodParameters(callMethod);
									route.callMethod(callMethod);
									data.add(new Route(route));
									return;
								}
								else if (cc == '\n')
								{
									throw exceptions::routes::IncompleteDeclaration(this->currentLine, this->currentChar);
								}
								else
								{
									stream << cc;
								}
							}
						}
						else if (cc == '\n')
						{
							throw exceptions::routes::IncompleteDeclaration(this->currentLine, this->currentChar);
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
				throw exceptions::routes::IncompleteDeclaration(this->currentLine, this->currentChar);
			}
			else
				stream << cc;
		}
		throw exceptions::PrematureEOF();
	}

	void runDoc(Routes &data, unsigned int level)
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
					throw exceptions::PrematureEOF();
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
					throw new exceptions::routes::InvalidChar(this->currentLine, this->currentChar);
				}
			}
			else if ((cc != '\n') && (cc != '\r') && (cc != ' ') && (cc != '\t'))
			{
				this->runLine(data, level);
			}
		}
	}
public:
	Parser(std::string inputFolder)
		: inputFolder(inputFolder), inputStreamBufferSize(0), currentInputStreamChar(0), currentLine(0), currentChar(0)
	{ }

	void compile(std::string inputFile, Routes &data)
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
			catch (std::ios_base::failure& e)
			{
				throw exceptions::OpenFile((boost::locale::format(boost::locale::translate("Cannot open file '{1}'")) % ifp).str(), &e);
			}
		}
		else
		{
			throw exceptions::FileNotFound(ifp.string());
		}
	}
};

boost::regex Parser::variablesName("[a-zA-Z_0-9]+");
}
}

#endif //ICARUSFRAMEWORK_ROUTES_PARSER_HPP
