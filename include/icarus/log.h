/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 21, 2016
 **/

#ifndef ICARUSFRAMEWORK_LOG_H
#define ICARUSFRAMEWORK_LOG_H

#include <boost/log/trivial.hpp>

#define LOG_ERROR(logdata)		BOOST_LOG_SEV(icarus::log::lg, boost::log::trivial::error)		<< icarus::log::fname(__FILE__) << ":" << __LINE__ << "] " << logdata;
#define LOG_WARNING(logdata)	BOOST_LOG_SEV(icarus::log::lg, boost::log::trivial::warning)	<< icarus::log::fname(__FILE__) << ":" << __LINE__ << "] " << logdata;
#define LOG_INFO(logdata)		BOOST_LOG_SEV(icarus::log::lg, boost::log::trivial::info)		<< icarus::log::fname(__FILE__) << ":" << __LINE__ << "] " << logdata;
#define LOG_TRACE(logdata)		BOOST_LOG_SEV(icarus::log::lg, boost::log::trivial::trace) 		<< icarus::log::fname(__FILE__) << ":" << __LINE__ << "] " << logdata;
#define LOG_DEBUG(logdata)		BOOST_LOG_SEV(icarus::log::lg, boost::log::trivial::debug) 		<< icarus::log::fname(__FILE__) << ":" << __LINE__ << "] " << logdata;

namespace icarus
{
namespace log
{
	static boost::log::sources::severity_logger<boost::log::trivial::severity_level> lg;

	const unsigned int SOURCE_DIRECTORY_LENGTH = strlen(SOURCE_DIRECTORY) + 1;

	const std::string fname(const std::string &name);
}
}

#endif //ICARUSFRAMEWORK_LOG_H
