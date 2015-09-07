/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#ifndef ICARUSFRAMEWORK_RESPONSEHANDLER_H
#define ICARUSFRAMEWORK_RESPONSEHANDLER_H

#include <simpleweb/server_http.hpp>
#include "headers.hpp"
#include "statuses.h"

namespace icarus
{
namespace http
{
class Response
{
public:
	static std::string endh;
private:
	std::ostream &responseStream;
	std::stringstream stream;
public:
	Response(std::ostream &responseStream)
		: responseStream(responseStream), stream(std::stringstream::binary | std::stringstream::in | std::stringstream::out)
	{ }

	~Response()
	{
		this->responseStream << "HTTP/1.1 " << this->status << " OK" << endh;

		bool contentLength = false;
		for (Header &header : this->headers)
		{
			if (header.name == "Content-Length")
				contentLength = true;
			this->responseStream << header.name << ": " << header.value << endh;
		}

		if (!contentLength)
			this->responseStream << "Content-Length: " << this->stream.tellp() << endh << endh;
		else
			this->responseStream << endh;

		this->stream.seekg(0, std::ios::beg);
		this->responseStream << this->stream.rdbuf();
	}

	Status status;
	Headers headers;

	template<class T>
	Response &operator<<(const T& t)
	{
		this->stream << t;
		return *this;
	}

	Response & operator<<(std::ostream& (*manip)(std::ostream&))
	{
		this->stream << manip;
		return *this;
	}

	Response & operator<<(Response & (*manip)(Response &))
	{
		return manip(*this);
	}
};
std::string Response::endh("\r\n");
}
}


#endif //ICARUSFRAMEWORK_RESPONSEHANDLER_H
