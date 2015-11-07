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

	virtual void write(std::ostream &stream, const Route &route) = 0;
	virtual void write(std::ostream &stream, const Group &group) = 0;

	virtual void write(std::ostream &stream, const Piece &piece)
	{ }
public:

	void write(std::ostream &stream, Routes &data)
	{
		this->writeBeginDoc(stream, data);
		for (const std::unique_ptr<Piece> piece : data.pieces())
		{
			Route *route = dynamic_cast<Route*>(piece.get());
			if (route)
				this->write(stream, *route);
			else
			{
				Group *group = dynamic_cast<Group*>(piece.get());
				if (group)
					this->write(stream, *group);
				else
					this->write(stream, *piece));
			}
		}
		this->writeEndDoc(stream, data);
	}
};
}
}


#endif //ICARUSFRAMEWORK_ROUTESWRITER_HPP
