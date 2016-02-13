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

	unsigned int _routesWritten;

	virtual void writeBeginDoc(std::ostream &stream, Document &document)
	{
		// File header
		stream << "/**" << std::endl << " * Auto generated routes file" << std::endl << " */" << std::endl << std::endl;

		// Twice inclusion fix
		std::string defconstName("ICARUSFRAMEWORK_ROUTESFILES_");
		stream << "#ifndef ";
		for (std::string package : document.packages())
		{
			defconstName += package;
			defconstName += "_";
		}
		defconstName += document.name();
		stream << defconstName << std::endl << "#define " << defconstName << std::endl << std::endl;

		// Default includes
		stream << "#include <icarus/routes/data.hpp>" << std::endl;
		stream << "#include <icarus/routes/to_url.hpp>" << std::endl << std::endl;

		// Namespaces
		stream << "namespace icarus" << std::endl << "{" << std::endl;
		for (std::string package : document.packages())
		{
			stream << "namespace " << package << std::endl << "{" << std::endl;
		}
		stream << "namespace " << document.name() << std::endl << "{" << std::endl;

		// Route instance initialization.
		Route *route;
		Group *group;
		for (const std::unique_ptr<Piece> &piece : document.pieces())
		{
			if (route = dynamic_cast<Route*>(piece.get()))			// If a route
			{
				this->writeBeginDoc(stream, *route);
			}
			else if (group = dynamic_cast<Group*>(piece.get()))		// If a group
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

		// Function declaration (interface)
		stream << "bool find(icarus::http::ClientContext &context)" << std::endl << "{" << std::endl;
		stream << "\tconst std::string method = context.request().method();" << std::endl;
		stream << "\tconst std::string uri = context.request().uri();" << std::endl;
		stream << "\tconst icarus::http::string uri = context.request().uri();" << std::endl;
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

	/**
	 * Write the initialization of the route.
	 */
	virtual void writeBeginDoc(std::ostream &stream, Route& route)
	{
		// If needed, sets the route id, a unique ID route per compilation.
		if (route.id() == 0)
			route.id(this->_routeId++);

		// Route comment
		stream << "/**" << std::endl << " * Route: " << route.uri().str() << std::endl << " * at line" << route.line()
			<< " on the original file." << std::endl << " */" << std::endl;
		// Route declaration itself
		stream << "icarus::routes::Route route" << route.id() << "(" << route.line() << ", {" << std::endl;
		unsigned int i = 0, j = 0;
		// Creating the static list initialization of the route with the match rule.
		for (RegexToken &token : route.uri().tokens())
		{
			if (i > 0)
				stream << "," << std::endl;
			if (!token.name().empty())
				token.index(j++);
			stream << "\t{\"" << token.name() << "\", \"" << token.regex() << "\"}";
			i++;
		}
		stream << std::endl << "});" << std::endl << std::endl;
	}

	/**
	 * Writes the end of the routing file.
	 */
	virtual void writeEndDoc(std::ostream &stream, Document &document)
	{
		stream << "} // find()" << std::endl;
		stream << "} // namespace " << document.name() << std::endl;
		for (std::string package : document.packages())
		{
			stream << "} // namespace " << package << std::endl;
		}
		stream << "} // namespace icarus" << std::endl;
	}

	/**
	 * Writes the evaluation of the URI for a route, if ok it will dispatch the method that needs to be called.
	 *
	 * @param route Route that will be evaluated.
	 */
	virtual void write(std::ostream &stream, Route &route)
	{
		stream << "\t";	// identation

		if (this->_routesWritten > 0)	// If any route was written before
			stream << "else ";			// Places an else dividing them.

		// If that evaluates if `route` matches.
		stream << "if (route" << route.id() << ".match(method, uri, values))" << std::endl << "\t{" << std::endl << "\t\t";

		// If the calling method is static (default).
		if (route.callMethod().isStatic())
		{
			// Places the packages of the method.
			for (const std::string &package : route.callMethod().path())
			{
				stream << package << "::";
			}
			// Places the method name
			stream << route.callMethod().name() << "(";
			unsigned int i = 0;
			bool found;
			// Places the parameter.
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
			stream << "\t\treturn true;" << std::endl;
		}
		else
		{
			LOG_ERROR("Non static calls are not implemented yet.");
			// TODO: Throw an exception
			// TODO: Implement it.
			std::exit(99);
		}
		stream << "\t}" << std::endl;
		// Increases the amount of written routes
		this->_routesWritten++;
	}

	/**
	 * Places all the routes of a group.
	 *
	 * @see write(std::ostream, Route&);
	 */
	virtual void write(std::ostream &stream, const Group &group)
	{
		for (const std::unique_ptr<Piece> &piece : group.pieces())
		{
			this->write(stream, *piece);
		}
	}

	/**
	 * Evaluates what `piece` is (class type) and call the proper `write` method for it.
	 */
	virtual void write(std::ostream &stream, Piece &piece)
	{
		Route *route;
		Group *group;
		if (route = dynamic_cast<Route*>(&piece))		// If it is a route
		{
			this->write(stream, *route);
		}
		else if (group = dynamic_cast<Group*>(&piece))	// If it is a group
		{
			this->write(stream, *route);
		}
		else											// If it is not a type expected, it should generate a fatal error
		{
			LOG_ERROR("Piece not supported.")
			// TODO: Throw an exception.
			std::exit(0);
		}
	}

	/**
	 * Write the reverse route for the document.
	 *
	 * @param @std::ostream Document
	 * @param icarus::routes::Document& Document that will be written
	 */
	virtual void writeReverseRoutes(std::ostream &stream, Document &document)
	{
		std::string item, del("::");
		for (const std::unique_ptr<Piece> &piece : document.pieces())
		{
			Group* group = dynamic_cast<Group*>(piece.get());
			if (group)
			{
				// TODO Implement.
			}
			else
			{
				Route* route = dynamic_cast<Route*>(piece.get());
				if (route)
				{
					this->writeReverseRoutes(stream, *route);
				}
				// TODO: Decide what to do with this else.
			}
		}
	}

	virtual void writeReverseRoutes(std::ostream &stream, Route &route)
	{
		for (std::string ns : route.callMethod().path())
		{
			stream << "namespace " << ns << "\n{\n";
		}
		stream << "namespace routes\n{\n";

		// String return version
		{
			stream << "\tstd::string " << route.callMethod().name() << "(";
			unsigned int i = 0;
			for (const icarus::routes::MethodParam &param : route.callMethod().params())
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
			for (const RegexToken &rt : route.uri().tokens())
			{
				if (rt.name().empty())
					stream << "\t\ttmp += \"" << rt.regex() << "\";\n";
				else
				{
					bool found = false;
					for (const MethodParam &p : route.callMethod().params())
					{
						if (rt.name() == p.name())
						{
							stream << "\t\ttmp += icarus::to_url(" << p.name() << ");\n";
							found = true;
							break;
						}
					}
					if (!found)
						throw exceptions::routes::InvalidParamName(route.line(), rt.name());
				}
			}
			stream << "\t\treturn tmp;\n";
			stream << "\t}\n";
		}

		// Action return version
		{
			stream << "\ticarus::icarus::Action _" << route.callMethod().name() << "(";
			unsigned int i = 0;
			for (const icarus::routes::MethodParam &param : route.callMethod().params())
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
			for (const RegexToken &rt : route.uri().tokens())
			{
				if (rt.name().empty())
					stream << "\t\ttmp += \"" << rt.regex() << "\";\n";
				else
				{
					bool found = false;
					for (const MethodParam &p : route.callMethod().params())
					{
						if (rt.name() == p.name())
						{
							stream << "\t\ttmp += icarus::to_url(" << p.name() << ");\n";
							found = true;
							break;
						}
					}
					if (!found)
						throw exceptions::routes::InvalidParamName(route.line(), rt.name());
				}
			}
			stream << "\t\treturn icarus::icarus::Action(\"" << route.httpMethod() << "\", tmp);\n";
			stream << "\t}\n";
		}
		stream << "} // routes\n";
		for (std::string ns : route.callMethod().path())
		{
			stream << "} // " << ns << "\n";
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

		stream << "#endif";
	}
};
}
}

#endif //ICARUSFRAMEWORK_ROUTESWRITER_HPP
