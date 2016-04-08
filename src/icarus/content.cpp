/**
 * @author J. Santos <jamillo@gmail.com>
 * @date March 14, 2016
 **/

#include <icarus/content.h>
#include <icarus/exceptions.h>
#include <icarus/log.h>

icarus::content::content()
	: _content_stream(new std::stringstream(std::stringstream::in | std::stringstream::out | std::stringstream::binary))
{
	LOG_DEBUG("icarus::content::content()");
}

icarus::content::content(const std::string &content)
	: icarus::content::content()
{
	LOG_DEBUG("icarus::content::content(const std::string &content)");
	(*this->_content_stream) << content;
}

icarus::content::content(icarus::content &html)
	: icarus::content::content()
{
	LOG_DEBUG("icarus::content::content(icarus::content &html)");
	(*this->_content_stream) << html._content_stream->rdbuf();
}

icarus::content::content(icarus::content &&html)
	: _content_stream(html._content_stream.release())
{
	LOG_DEBUG("icarus::content::content(icarus::content &&html)");
}

icarus::content::~content()
{ }

size_t icarus::content::size()
{
	if (!this->_content_stream)
		throw icarus::invalid_pointer((bl::translate("Invalid internal stream.")).str());
	return this->_content_stream->tellp();
}

std::stringstream &icarus::content::stream() const
{
	if (!this->_content_stream)
		throw icarus::invalid_pointer((bl::translate("Invalid internal stream.")).str());
	return *this->_content_stream;
}
