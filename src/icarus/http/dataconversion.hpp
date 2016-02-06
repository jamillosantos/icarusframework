/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 05, 2016
 **/

#ifndef ICARUSFRAMEWORK_DATACONVERSION_HPP
#define ICARUSFRAMEWORK_DATACONVERSION_HPP

#include <string>

namespace icarus
{
namespace data
{

void fromString(int8_t &dest, const std::string &string)
{
	dest = std::stoi(string);
}

void fromString(short int &dest, const std::string &string)
{
	dest = std::stoi(string);
}

void fromString(int &dest, const std::string &string)
{
	dest = std::stoi(string);
}

void fromString(long int &dest, const std::string &string)
{
	dest = std::stoi(string);
}

void fromString(long long int &dest, const std::string &string)
{
	dest = std::stoi(string);
}

void fromString(uint8_t &dest, const std::string &string)
{
	dest = std::stoi(string);
}

void fromString(unsigned short int &dest, const std::string &string)
{
	dest = std::stoi(string);
}

void fromString(unsigned int &dest, const std::string &string)
{
	dest = std::stoi(string);
}

void fromString(unsigned long int &dest, const std::string &string)
{
	dest = std::stoi(string);
}

void fromString(unsigned long long int &dest, const std::string &string)
{
	dest = std::stoi(string);
}

}
}

#endif //ICARUSFRAMEWORK_DATACONVERSION_HPP
