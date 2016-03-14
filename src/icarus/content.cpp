/**
 * @author J. Santos <jamillo@gmail.com>
 * @date March 14, 2016
 **/

#include "icarus/content.h"

icarus::content::content()
	: _content_stream(std::stringstream::binary | std::stringstream::in | std::stringstream::out)
{ }

icarus::content::content(const std::string &content)
{
	this->_content_stream << content;
}

icarus::content::content(icarus::content &html)
	: icarus::content::content()
{
	this->_content_stream << html._content_stream.rdbuf();
}

icarus::content::content(icarus::content &&html)
	: _content_stream(std::move(html._content_stream))
{ }

icarus::content::~content()
{ }

size_t icarus::content::size()
{
	return this->_content_stream.tellp();
}

std::stringstream &icarus::content::stream()
{
	return this->_content_stream;
}
