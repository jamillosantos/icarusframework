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
	unsigned int _routeId;

	virtual void writeBeginDoc(std::ostream &stream, Document &document)
	{
		for (std::string package : document.packages())
		{
			stream << "namespace " << package << std::endl << "{" << std::endl;
		}

		Route *route;
		Group *group;
		for (const std::unique_ptr<Piece> &piece : document.pieces())
		{
			if (route = dynamic_cast<Route*>(piece.get()))
			{
				this->writeBeginDoc(stream, *route);
			}
			else if (group = dynamic_cast<Group*>(piece.get()))
			{
				this->writeBeginDoc(stream, *group);
			}
			else
			{
				LOG_ERROR("Type error on writing routes (" << piece->line() << ").");
				// TODO: Throw an exception.
				std::exit(0);
			}
		}
	};

	virtual void writeBeginDoc(std::ostream &stream, Group& group)
	{
		Route *route;
		Group *subgroup;
		for (const std::unique_ptr<Piece> &piece : group.pieces())
		{
			if (route = dynamic_cast<Route*>(piece.get()))
			{
				this->writeBeginDoc(stream, *route);
			}
			else if (subgroup = dynamic_cast<Group*>(piece.get()))
			{
				this->writeBeginDoc(stream, *subgroup);
			}
			else
			{
				LOG_ERROR("Type error on writing routes (" << piece->line() << ").");
				// TODO: Throw an exception.
			}
		}
	}

	virtual void writeBeginDoc(std::ostream &stream, Route& route)
	{
		if (route.id() == 0)
			route.id(this->_routeId++);
		stream << "\ticarus::routes::Route route" << route.id() << "(" << route.line() << ", {" << std::endl;
		unsigned int i = 0, j = 0;
		for (RegexToken &token : route.uri().tokens())
		{
			if (i > 0)
				stream << "," << std::endl;
			if (!token.name().empty())
				token.index(j++);
			stream << "\t\t{\"" << token.name() << "\", \"" << token.regex() << "\"}";
			i++;
		}
		stream << std::endl << "\t});" << std::endl;
	}

	virtual void writeEndDoc(std::ostream &stream, Document &document)
	{
		for (std::string package : document.packages())
		{
			stream << "} // " << package << std::endl;
		}
	}

	virtual void write(std::ostream &stream, Route &route)
	{
		stream << "\t\tif (route" << route.id() << ".match(method, uri, values))" << std::endl << "\t\t{" << std::endl << "\t\t\t";
		if (route.callMethod().isStatic())
		{
			for (const std::string &package : route.callMethod().path())
			{
				stream << package << "::";
			}
			stream << route.callMethod().name() << "(";
			unsigned int i = 0;
			bool found;
			for (MethodParam &param : route.callMethod().params())
			{
				found = false;
				for (RegexToken &token : route.uri().tokens())
				{
					if (token.name() == param.name())
					{
						if (i++ > 0)
						{
							stream << ", ";
						}
						if ((param.type() == "") || (param.type() == "std::string") || (param.type() == "string"))
							stream << "values.get(" << token.index() << ")";
						else
							stream << "icarus::data::fromString(values.get(" << token.index() << "))";
						found = true;
						break;
					}
				}
				if (!found)
				{
					LOG_ERROR("Param " << param.name() << " was not found at line " << route.line() << ".");
					// TODO: Throw an exception.
					std::exit(99);
				}
			}
			stream << ");" << std::endl;
		}
		else
		{
			LOG_ERROR("Non static calls are not implemented yet.");
			// TODO: Throw an exception
			// TODO: Implement it.
			std::exit(99);
		}
		stream << "\t\t}" << std::endl << std::endl;
	}

	virtual void write(std::ostream &stream, const Group &group)
	{
		for (const std::unique_ptr<Piece> &piece : group.pieces())
		{
			this->write(stream, *piece);
		}
	}

	virtual void write(std::ostream &stream, Piece &piece)
	{
		Route *route;
		Group *group;
		if (route = dynamic_cast<Route*>(&piece))
		{
			this->write(stream, *route);
		}
		else if (group = dynamic_cast<Group*>(&piece))
		{
			this->write(stream, *route);
		}
		else
		{
			LOG_ERROR("Piece not supported.")
			// TODO: Throw an exception.
			std::exit(0);
		}
	}

	virtual void writeReverseRoutes(std::ostream &stream, Document &document)
	{
		std::string item, del("::");
		for (const std::unique_ptr<Piece> &piece : document.pieces())
		{
			Group* group = dynamic_cast<Group*>(piece.get());
			if (group)
			{
				// TODO Implement this part.
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
					stream << "\ticarus::icarus::Action " << route->callMethod().name() << "(";
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
					stream << "\t\treturn icarus::icarus::Action(\"" << route->httpMethod() << "\", tmp);\n";
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
	RoutesWriter()
		: _routeId(0)
	{ }

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
