/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date September 09, 2015
 */

#ifndef ICARUSFRAMEWORK_ROUTESWRITER_HPP
#define ICARUSFRAMEWORK_ROUTESWRITER_HPP

#include <ostream>
#include <sstream>
#include "data.hpp"

namespace icarus
{
namespace routes
{
class RoutesWriter
{
protected:
	virtual void writeBeginDoc(std::ostream &stream, Document &document) = 0;
	virtual void writeEndDoc(std::ostream &stream, Document &document) = 0;

	virtual void write(std::ostream &stream, const Route &route) = 0;
	virtual void write(std::ostream &stream, const Group &group) = 0;

	virtual void write(std::ostream &stream, const Piece &piece)
	{ }

	virtual void writeReverseRoutes(std::ostream &stream, Document &document)
	{
		std::string item, del("::");
		for (const std::unique_ptr<Piece> &piece : document.pieces())
		{
			Group* group = dynamic_cast<Group*>(piece.get());
			if (group)
			{
				//
			}
			else
			{
				Route* route = dynamic_cast<Route*>(piece.get());
				if (route)
				{
					for (std::string ns : route->callMethod().path())
					{
						stream << "namespace " << ns << "\n{\n";
					}
					stream << "namespace routes\n{\n";
					stream << "\ticarus::framework::Action " << route->callMethod().name() << "(";
					unsigned int i = 0;
					for (const icarus::routes::MethodParam &param : route->callMethod().params())
					{
						if (i > 0)
							stream << ", ";
						stream << param.type() << " ";
						if (param.attribute() == icarus::routes::MethodParamType::POINTER)
							stream << "*";
						if (param.attribute() == icarus::routes::MethodParamType::REFERENCE)
							stream << "&";
						stream << param.name();
						i++;
					}
					stream << ")\n\t{\n";
					stream << "\t\tstd::string tmp;\n";
					for (const RegexToken &rt : route->uri().tokens())
					{
						if (rt.name().empty())
							stream << "\t\ttmp += \"" << rt.regex() << "\";\n";
						else
						{
							bool found = false;
							for (const MethodParam &p : route->callMethod().params())
							{
								if (rt.name() == p.name())
								{
									stream << "\t\ttmp += icarus::to_url(" << p.name() << ");\n";
									found = true;
									break;
								}
							}
							if (!found)
								throw exceptions::routes::InvalidParamName(route->line(), rt.name());
						}
					}
					stream << "\t\treturn icarus::framework::Action(\"" << route->httpMethod() << "\", tmp);\n";
					stream << "\t}\n";
					stream << "} // routes\n";
					for (std::string ns : route->callMethod().path())
					{
						stream << "} // " << ns << "\n";
					}
				}
				// TODO: Decide what to do with this else.
			}
		}
	}
public:

	virtual void write(std::ostream &stream, Document &document)
	{
		this->writeBeginDoc(stream, document);
		for (const std::unique_ptr<Piece> &piece : document.pieces())
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
					this->write(stream, *piece);
			}
		}
		this->writeEndDoc(stream, document);

		this->writeReverseRoutes(stream, document);
	}
};
}
}

#endif //ICARUSFRAMEWORK_ROUTESWRITER_HPP
