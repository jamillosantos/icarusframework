/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#ifndef ICARUSFRAMEWORK_ROUTES_COMPILER_HPP
#define ICARUSFRAMEWORK_ROUTES_COMPILER_HPP

#define INPUT_STREAM_BUFFER_SIZE 4096

#include <string>
#include <boost/filesystem.hpp>
#include <stdexcept>
#include <fstream>
#include <sstream>
#include "data.hpp"

#include <boost/regex.hpp>

namespace icarus
{
namespace routes
{
class Parser
{
private:
	static boost::regex variablesName;

	std::vector<RoutesLine> _lines;

	std::unique_ptr<std::istream> istream;

	std::string outputFolder;

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
		// TODO: Specialize exception
		std::cerr << "Unexpected end of file." << std::endl;
		throw new std::exception();
	}

	std::string readUntil(std::string chars)
	{
		char cc;
		std::string result;
		while (this->readChar(&cc))
		{
			if (result.find(cc) == std::string::npos)
				result += cc;
			else
				return result;
		}
		// TODO: Specialize exception
		std::cerr << "Unexpected end of file." << std::endl;
		throw new std::exception();
	}

	void runCommentLine()
	{
		char cc;
		while (this->readChar(&cc))
		{
			if (cc == '\n')
				break;
		}
	}

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
		// TODO: Specialize exception
		std::cerr << "Unexpected end of file." << std::endl;
		throw std::exception();
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
		// TODO: Specialize exception
		std::cerr << "Unexpected end of file." << std::endl;
		throw std::exception();
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
					callMethod.addParam(paramstream.str(), MethodParam::MethodType::NORMAL, stream.str());
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
					// TODO: Specialize exception
					std::cerr << "Unexpected end of file." << std::endl;
					throw std::exception();
				}
			}
		}
	}

	void runLine()
	{
		this->_lines.emplace_back(this->currentLine);
		RoutesLine &line = this->_lines.back();
		std::stringstream stream;
		char cc = this->lastChar();
		stream << cc;
		while (this->readChar(&cc))
		{
			if ((cc == ' ') || (cc == '\t'))
			{
				line.httpMethod(stream.str());
				stream.str("");

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
							// TODO: Specialize exception
							std::cerr << "Unexpected end of file." << std::endl;
							throw std::exception();
						}
					}
					else if (cc == '<')
					{
						bool success = false;
						std::string tmpstr = stream.str();
						if (!tmpstr.empty())
						{
							line.addToken(tmpstr);
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
										line.addToken(tmpstr, stream.str());
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
								line.addToken(stream.str(), "");
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
								// TODO: specialize exception
								std::cerr << "Invalid char." << std::endl;
								throw std::exception();
							}
						}
						if(!success)
						{
							// TODO: specialize exception
							std::cerr << "Unexpected end of file." << std::endl;
							throw std::exception();
						}
					}
					else if ((cc == ' ') || (cc == '\t'))
					{
						std::string tmpstr = stream.str();
						if (!tmpstr.empty())
							line.addToken(tmpstr);
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
								line.callMethod(callMethod);
								return;
							}
							else if (cc == '\n')
							{
								// TODO: specialize exception
								std::cerr << "Route declaration incomplete." << std::endl;
								throw std::exception();
							}
							else
							{
								stream << cc;
							}
						}
					}
					else if (cc == '\n')
					{
						// TODO: specialize exception
						std::cerr << "Route declaration incomplete." << std::endl;
						throw std::exception();
					}
					else
					{
						stream << cc;
					}
				}
			}
			else if (cc == '\n')
			{
				// TODO: Specialize exception
				std::cerr << "Incomplete declaration." << std::endl;
				throw std::exception();
			}
			else
				stream << cc;
		}
		// TODO: Specialize exception
		std::cerr << "Unexpected end of file." << std::endl;
		throw std::exception();
	}

	void runDoc()
	{
		char cc;
		while (this->readChar(&cc))
		{
			if (cc == '/')
			{
				if (this->readChar(&cc))
				{
					if (cc == '/')
					{
						this->runCommentLine();
					}
					else if (cc == '*')
					{
						this->runCommentMultiLine();
					}
				}
				else
				{
					// Throw an exception
					std::cerr << "Unexpected end of file." << std::endl;
					throw std::exception();
				}
			}
			else if ((cc != '\n') && (cc != '\r') && (cc != ' ') && (cc != '\t'))
			{
				this->runLine();
			}
		}
	}
public:
	Parser()
		: inputStreamBufferSize(0), currentInputStreamChar(0), currentLine(0), currentChar(0)
	{ }

	const std::vector<RoutesLine> &lines() const
	{
		return this->_lines;
	}

	void compile(std::string inputFile)
	{
		boost::filesystem::path ifp(inputFile);
		if (boost::filesystem::exists(ifp))
		{
			this->istream.reset(new std::ifstream(ifp.string()));
			if (this->istream)
			{
				this->runDoc();
			}
			else
			{
				// TODO: specialize exception
				std::cerr << "Cannot open file." << std::endl;
				throw std::exception();
			}
		}
		else
		{
			// TODO: specialize exception
			std::cerr << "Input file doesn't exists." << std::endl;
			throw std::exception();
		}
	}
};

boost::regex Parser::variablesName("[a-zA-Z_0-9]+");
}
}

#endif //ICARUSFRAMEWORK_HTTP_STATUSES_H
