/**
 * @author J. Santos <jamillo@gmail.com>
 * @date March 14, 2016
 **/

#ifndef ICARUSFRAMEWORK_HTML_H
#define ICARUSFRAMEWORK_HTML_H

#include <sstream>
#include <string>

namespace icarus
{
class content
{
private:
	std::stringstream _content_stream;

public:
	content();

	content(const std::string &content);

	content(icarus::content &html);

	content(icarus::content &&html);

	virtual ~content();

	size_t size();

	std::stringstream & stream();

	template<typename T>
	icarus::content &operator<<(const T &t)
	{
		this->_content_stream << t;
		return *this;
	}

	icarus::content &operator<<(icarus::content &html)
	{
		this->_content_stream << html._content_stream.rdbuf();
		return *this;
	}

	icarus::content &operator<<(std::ostream &(*manip)(std::ostream &))
	{
		this->_content_stream << manip;
		return *this;
	}
};
}

#endif //ICARUSFRAMEWORK_HTML_H
