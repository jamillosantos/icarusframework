/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 21, 2016
 **/

#ifndef ICARUSFRAMEWORK_LOG_HPP
#define ICARUSFRAMEWORK_LOG_HPP

#include <boost/log/trivial.hpp>

#define LOG_ERROR(logdata)		BOOST_LOG_SEV(icarus::log::lg, boost::log::trivial::error)		<< logdata;
#define LOG_WARNING(logdata)	BOOST_LOG_SEV(icarus::log::lg, boost::log::trivial::warning)	<< logdata;
#define LOG_INFO(logdata)		BOOST_LOG_SEV(icarus::log::lg, boost::log::trivial::info)		<< logdata;
#define LOG_TRACE(logdata)		BOOST_LOG_SEV(icarus::log::lg, boost::log::trivial::trace) 		<< logdata;

namespace icarus
{
namespace log
{
	boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;
}
}

#endif //ICARUSFRAMEWORK_LOG_HPP
