/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 17, 2015
 */

#ifndef MINOTAUR_OCHAINSTREAM_H
#define MINOTAUR_OCHAINSTREAM_H

#include <string>
#include <vector>

namespace minotaur
{
class OChainStream
{
private:
	std::vector<std::unique_ptr<std::stringstream>> content;
public:
	OChainStream()
	{
		this->content.emplace_back(new std::stringstream(std::stringstream::binary | std::stringstream::in | std::stringstream::out));
	}

	OChainStream(OChainStream&& stream)
	{
		std::swap(stream.content, this->content);
	}

	template<typename T>
	OChainStream &operator<<(const T o)
	{
		(*this->content.back()) << o;
	}

	template<typename T>
	OChainStream &operator<<(const OChainStream &o)
	{
		for (std::unique_ptr<std::stringstream> s : o.content)
		{
			this->content.push_back(std::move(s));
		}
		o.content.clear();
	}

	template<typename T>
	OChainStream &operator<<(const std::unique_ptr<OChainStream> &o)
	{
		this << *o;
	}
};
};


#endif // MINOTAUR_OCHAINSTREAM_H
