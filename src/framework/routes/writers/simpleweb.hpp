/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date September 09, 2015
 */

#ifndef ICARUSFRAMEWORK_SIMPLEWEB_HPP
#define ICARUSFRAMEWORK_SIMPLEWEB_HPP

#include <simpleweb/server_http.hpp>
#include "../routeswriter.hpp"

namespace icarus
{
namespace framework
{
namespace routes
{
namespace writers
{

class SimpleWebApply
{
public:
	// void apply(SimpleWeb::ServerBase);
};

class SimpleWebWriter
	: public RoutesWriter
{
protected:

	virtual void write(std::ostream &stream, const Group &group) override
	{ }

	virtual void writeBeginDoc(std::ostream &stream, Document &document) override
	{
		stream << "/**" << std::endl;
		stream << " * Auto generated." << std::endl;
		stream << " **/" << std::endl << std::endl;
		stream << "namespace icarus" << std::endl << "{" << std::endl;
		stream << "namespace generated" << std::endl << "{" << std::endl;
		stream << "namespace routes" << std::endl << "{" << std::endl;
		stream << "namespace writers" << std::endl << "{" << std::endl;
		for (std::string &package : document.packages())
		{
			stream << "namespace " << package << std::endl << "{" << std::endl;
		}
		stream << "class " << document.name() << std::endl << "{" << std::endl;
		stream << "\ttemplate <class socket_type>" << std::endl << "{" << std::endl;
		stream << "\tvoid apply(SimpleWeb::ServerBase<socket_type> &server)" << document.name() << std::endl << "{" << std::endl;
	}

	virtual void writeEndDoc(std::ostream &stream, Document &document) override
	{
		stream << "}; // " << document.name() << std::endl;
		for (std::string &package : document.packages())
		{
			stream << "} // " << package << std::endl;
		}
		stream << "} // icarus" << std::endl;
		stream << "} // generated" << std::endl;
		stream << "} // routes" << std::endl;
		stream << "} // writers" << std::endl;
	}

	virtual void write(std::ostream &stream, const Route &route) override
	{
		// stream << "\tserver.resources[\"" << route.httpMethod() << "\"][]" << std::endl;
	}
public:

	virtual void write(std::ostream &stream, Document &document) override
	{
		RoutesWriter::write(stream, document);
	}
};
} // writers
} // routes
} // framework
} // icarus


#endif //ICARUSFRAMEWORK_SIMPLEWEB_HPP
