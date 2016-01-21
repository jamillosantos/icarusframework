/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 21, 2016
 **/

#ifndef ICARUSFRAMEWORK_LOG_HPP
#define ICARUSFRAMEWORK_LOG_HPP

#include <boost/log/trivial.hpp>

namespace icarus
{
class Log
{
private:
	boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;
public:
	template<class T>
	Log &operator<<(const T& t)
	{
		BOOST_LOG_SEV(lg, boost::log::trivial::info) << t;

		return *this;
	}
};

Log log;
}

#endif //ICARUSFRAMEWORK_LOG_HPP
