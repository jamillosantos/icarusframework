/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date September 09, 2015
 */

#include <icarus/routes/routeswriter.h>

#include <icarus/routes/exceptions.h>

void icarus::routes::routes_writer::writeBeginDoc(std::ostream &stream, document &document)
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
	route *route;
	group *group;
	for (const std::unique_ptr<piece> &piece : document.pieces())
	{
		if ((route = dynamic_cast<icarus::routes::route*>(piece.get())))			// If a route
		{
			this->writeBeginDoc(stream, *route);
		}
		else if ((group = dynamic_cast<icarus::routes::group*>(piece.get())))		// If a group
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
	stream << "bool find(icarus::http::client_context &context)" << std::endl << "{" << std::endl;
	stream << "\tconst std::string method = context.request().method();" << std::endl;
	stream << "\tconst std::string uri = context.request().uri();" << std::endl;
	stream << "\tconst icarus::http::string uri = context.request().uri();" << std::endl;
};

void icarus::routes::routes_writer::writeBeginDoc(std::ostream &stream, icarus::routes::group& group)
{
	icarus::routes::route *route;
	icarus::routes::group *subgroup;
	for (const std::unique_ptr<icarus::routes::piece> &piece : group.pieces())
	{
		if ((route = dynamic_cast<icarus::routes::route*>(piece.get())))
		{
			this->writeBeginDoc(stream, *route);
		}
		else if ((subgroup = dynamic_cast<icarus::routes::group*>(piece.get())))
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

void icarus::routes::routes_writer::writeBeginDoc(std::ostream &stream, icarus::routes::route& route)
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
	for (regex_token &token : route.uri().tokens())
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

void icarus::routes::routes_writer::writeEndDoc(std::ostream &stream, icarus::routes::document &document)
{
	stream << "\treturn false;" << std::endl;
	stream << "} // find()" << std::endl;
	stream << "} // namespace " << document.name() << std::endl;
	for (std::string package : document.packages())
	{
		stream << "} // namespace " << package << std::endl;
	}
	stream << "} // namespace icarus" << std::endl;
}

void icarus::routes::routes_writer::write(std::ostream &stream, icarus::routes::route &route)
{
	stream << "\t";	// identation

	if (this->_routes_written > 0)	// If any route was written before
		stream << "else ";			// Places an else dividing them.

	// If that evaluates if `route` matches.
	stream << "if (route" << route.id() << ".match(method, uri, values))" << std::endl << "\t{" << std::endl << "\t\t";

	// If the calling method is static (default).
	if (route.callMethod().isStatic())
	{
		// Places the packages of the method.
		stream << "context.response() << ";
		for (const std::string &package : route.callMethod().path())
		{
			stream << package << "::";
		}
		// Places the method name
		stream << route.callMethod().name() << "(";
		unsigned int i = 0;
		bool found;
		// Places the parameter.
		for (method_param &param : route.callMethod().params())
		{
			found = false;
			for (regex_token &token : route.uri().tokens())
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
				throw icarus::routes::param_not_found(route.line(), param.name());
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
	this->_routes_written++;
}

void icarus::routes::routes_writer::write(std::ostream &stream, const icarus::routes::group &group)
{
	for (const std::unique_ptr<piece> &piece : group.pieces())
	{
		this->write(stream, *piece);
	}
}

void icarus::routes::routes_writer::write(std::ostream &stream, icarus::routes::piece &piece)
{
	icarus::routes::route *route;
	icarus::routes::group *group;
	if ((route = dynamic_cast<icarus::routes::route*>(&piece)))		// If it is a route
	{
		this->write(stream, *route);
	}
	else if ((group = dynamic_cast<icarus::routes::group*>(&piece)))	// If it is a group
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

void icarus::routes::routes_writer::writeReverseRoutes(std::ostream &stream, icarus::routes::document &document)
{
	std::string item, del("::");
	for (const std::unique_ptr<icarus::routes::piece> &piece : document.pieces())
	{
		icarus::routes::group* group = dynamic_cast<icarus::routes::group*>(piece.get());
		if (group)
		{
			// TODO Implement.
		}
		else
		{
			icarus::routes::route* route = dynamic_cast<icarus::routes::route*>(piece.get());
			if (route)
			{
				this->writeReverseRoutes(stream, *route);
			}
			// TODO: Decide what to do with this else.
		}
	}
}

void icarus::routes::routes_writer::writeReverseRoutes(std::ostream &stream, icarus::routes::route &route)
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
		for (const icarus::routes::method_param &param : route.callMethod().params())
		{
			if (i > 0)
				stream << ", ";
			stream << param.type() << " ";
			if (param.attribute() == icarus::routes::method_param_type::POINTER)
				stream << "*";
			if (param.attribute() == icarus::routes::method_param_type::REFERENCE)
				stream << "&";
			stream << param.name();
			i++;
		}
		stream << ")\n\t{\n";
		stream << "\t\tstd::string tmp;\n";
		for (const icarus::routes::regex_token &rt : route.uri().tokens())
		{
			if (rt.name().empty())
				stream << "\t\ttmp += \"" << rt.regex() << "\";\n";
			else
			{
				bool found = false;
				for (const icarus::routes::method_param &p : route.callMethod().params())
				{
					if (rt.name() == p.name())
					{
						stream << "\t\ttmp += icarus::to_url(" << p.name() << ");\n";
						found = true;
						break;
					}
				}
				if (!found)
					throw icarus::routes::param_not_found(route.line(), rt.name());
			}
		}
		stream << "\t\treturn tmp;\n";
		stream << "\t}\n";
	}

	// Action return version
	{
		stream << "\ticarus::icarus::Action _" << route.callMethod().name() << "(";
		unsigned int i = 0;
		for (const icarus::routes::method_param &param : route.callMethod().params())
		{
			if (i > 0)
				stream << ", ";
			stream << param.type() << " ";
			if (param.attribute() == icarus::routes::method_param_type::POINTER)
				stream << "*";
			if (param.attribute() == icarus::routes::method_param_type::REFERENCE)
				stream << "&";
			stream << param.name();
			i++;
		}
		stream << ")\n\t{\n";
		stream << "\t\tstd::string tmp;\n";
		for (const icarus::routes::regex_token &rt : route.uri().tokens())
		{
			if (rt.name().empty())
				stream << "\t\ttmp += \"" << rt.regex() << "\";\n";
			else
			{
				bool found = false;
				for (const icarus::routes::method_param &p : route.callMethod().params())
				{
					if (rt.name() == p.name())
					{
						stream << "\t\ttmp += icarus::to_url(" << p.name() << ");\n";
						found = true;
						break;
					}
				}
				if (!found)
					throw icarus::routes::param_not_found(route.line(), rt.name());
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

icarus::routes::routes_writer::routes_writer()
	: _routeId(0)
{ }

void icarus::routes::routes_writer::write(std::ostream &stream, icarus::routes::document &document)
{
	this->writeBeginDoc(stream, document);
	for (const std::unique_ptr<piece> &piece : document.pieces())
	{
		icarus::routes::route *route = dynamic_cast<icarus::routes::route*>(piece.get());
		if (route)
			this->write(stream, *route);
		else
		{
			icarus::routes::group *group = dynamic_cast<icarus::routes::group*>(piece.get());
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
