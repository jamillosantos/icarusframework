/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date September 09, 2015
 */

#ifndef ICARUSFRAMEWORK_SIMPLEWEB_HPP
#define ICARUSFRAMEWORK_SIMPLEWEB_HPP

#include "../routeswriter.hpp"

namespace icarus
{
namespace routes
{
namespace writers
{
class SimpleWeb
	: public RoutesWriter
{
protected:
	virtual void writeBeginDoc(std::ostream &stream, Routes &data)
	{
		stream << "/**" << std::endl;
		stream << " * Auto generated." << std::endl;
		stream << " **/" << std::endl << std::endl;
		stream << "class " << std::endl;
	}

	virtual void writeEndDoc(std::ostream &stream, Routes &data)
	{

	}
};
}
}
}


#endif //ICARUSFRAMEWORK_SIMPLEWEB_HPP
