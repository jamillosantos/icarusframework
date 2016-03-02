/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 05, 2016
 **/

#include <icarus/http/dataconversion.h>

void icarus::data::from_string(int8_t &dest, const std::string &string)
{
	dest = std::stoi(string);
}

void icarus::data::from_string(short int &dest, const std::string &string)
{
	dest = std::stoi(string);
}

void icarus::data::from_string(int &dest, const std::string &string)
{
	dest = std::stoi(string);
}

void icarus::data::from_string(long int &dest, const std::string &string)
{
	dest = std::stoi(string);
}

void icarus::data::from_string(long long int &dest, const std::string &string)
{
	dest = std::stoi(string);
}

void icarus::data::from_string(uint8_t &dest, const std::string &string)
{
	dest = std::stoi(string);
}

void icarus::data::from_string(unsigned short int &dest, const std::string &string)
{
	dest = std::stoi(string);
}

void icarus::data::from_string(unsigned int &dest, const std::string &string)
{
	dest = std::stoi(string);
}

void icarus::data::from_string(unsigned long int &dest, const std::string &string)
{
	dest = std::stoi(string);
}

void icarus::data::from_string(unsigned long long int &dest, const std::string &string)
{
	dest = std::stoi(string);
}
