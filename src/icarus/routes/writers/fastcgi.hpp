/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 21, 2016
 **/

#ifndef ICARUSFRAMEWORK_ROUTES_WRITERS_FCGI_HPP
#define ICARUSFRAMEWORK_ROUTES_WRITERS_FCGI_HPP

#include "../routeswriter.hpp"

namespace icarus
{
namespace routes
{
namespace writers
{
class FastCGI
	: public RoutesWriter
{

protected:
	virtual void writeBeginDoc(std::ostream &stream, Document &document) override
	{ }

	virtual void writeEndDoc(std::ostream &stream, Document &document) override
	{ }

	virtual void write(std::ostream &stream, const Route &route) override
	{ }

	virtual void write(std::ostream &stream, const Group &group) override
	{ }
};
}
}
}

#endif //ICARUSFRAMEWORK_ROUTES_WRITERS_FCGI_HPP
