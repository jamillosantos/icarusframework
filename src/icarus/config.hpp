/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 24, 2016
 **/

#ifndef ICARUSFRAMEWORK_CONFIG_HPP
#define ICARUSFRAMEWORK_CONFIG_HPP

#include <string>

namespace icarus
{
namespace config
{
class Config
{
private:
	unsigned int _requestThreads;
public:
	unsigned int requestThreads()
	{
		return this->_requestThreads;
	}

	Config &requestThreads(unsigned int requestThreads)
	{
		this->_requestThreads = requestThreads;
		return *this;
	}
};
}
}

#endif //ICARUSFRAMEWORK_CONFIG_HPP
