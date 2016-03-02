/**
 * @author J. Santos <jamillo@gmail.com>
 * @date January 23, 2016
 **/

#include <icarus/routes/fieldtypes.h>

icarus::routes::field_types::field_types()
{
	this->set("uint8_t", "[0-9]+");
	this->set("uint16_t", "[0-9]+");
	this->set("uint32_t", "[0-9]+");
	this->set("uint64_t", "[0-9]+");
	this->set("unsigned byte", "[0-9]+");
	this->set("unsigned short", "[0-9]+");
	this->set("unsigned int", "[0-9]+");
	this->set("unsigned long", "[0-9]+");
	this->set("unsigned long long", "[0-9]+");

	this->set("int8_t", "-?[0-9]+");
	this->set("int16_t", "-?[0-9]+");
	this->set("int32_t", "-?[0-9]+");
	this->set("int64_t", "-?[0-9]+");
	this->set("byte", "-?[0-9]+");
	this->set("short", "-?[0-9]+");
	this->set("int", "-?[0-9]+");
	this->set("long", "-?[0-9]+");
	this->set("long long", "-?[0-9]+");
}

const std::string &icarus::routes::field_types::get(const std::string &typeName)
{
	auto search = this->_container.find(typeName);
	if (search == this->_container.end())
		return emptyString;
	else
		return (*search).second;
}

void icarus::routes::field_types::set(const std::string &typeName, const std::string regex)
{
	auto search = this->_container.find(typeName);
	if (search == this->_container.end())
		this->_container.emplace(std::make_pair(typeName, regex));
	else
		(*search).second = regex;
}

std::string icarus::routes::field_types::emptyString("");

icarus::routes::field_types icarus::routes::field_types::types;
