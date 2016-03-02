/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 23, 2016
 **/

#ifndef ICARUSFRAMEWORK_ROUTES_FIELDTYPES_HPP
#define ICARUSFRAMEWORK_ROUTES_FIELDTYPES_HPP

#include <map>

namespace icarus
{
namespace routes
{
class field_types
{
public:
	static field_types types;
private:
	std::map<std::string, std::string> _container;

	static std::string emptyString;
public:
	field_types();

	const std::string &get(const std::string &typeName);

	void set(const std::string &typeName, const std::string regex);
};
}
}

#endif //ICARUSFRAMEWORK_ROUTES_FIELDTYPES_HPP
