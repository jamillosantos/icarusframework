/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date September 09, 2015
 */

#ifndef ICARUSFRAMEWORK_ROUTESWRITER_HPP
#define ICARUSFRAMEWORK_ROUTESWRITER_HPP

#include <ostream>
#include "data.hpp"

namespace icarus
{
namespace routes
{
class RoutesWriter
{
protected:
	virtual void writeBeginDoc(std::ostream &stream, Routes &data) = 0;
	virtual void writeEndDoc(std::ostream &stream, Routes &data) = 0;

	virtual void writeLine(std::ostream &stream, const Route &line) = 0;
public:

	void write(std::ostream &stream, Routes &data)
	{
		this->writeBeginDoc(stream, data);
		for (const Route &line : data.lines())
			this->writeLine(stream, line);
		this->writeEndDoc(stream, data);
	}
};
}
}


#endif //ICARUSFRAMEWORK_ROUTESWRITER_HPP
