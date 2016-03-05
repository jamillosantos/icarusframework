/**
 * @author J. Santos <jamillo@gmail.com>
 * @date February 05, 2016
 **/

#ifndef ICARUSFRAMEWORK_DATACONVERSION_H
#define ICARUSFRAMEWORK_DATACONVERSION_H

#include <string>

namespace icarus
{
namespace data
{

void from_string(int8_t &dest, const std::string &string);

void from_string(short int &dest, const std::string &string);

void from_string(int &dest, const std::string &string);

void from_string(long int &dest, const std::string &string);

void from_string(long long int &dest, const std::string &string);

void from_string(uint8_t &dest, const std::string &string);

void from_string(unsigned short int &dest, const std::string &string);

void from_string(unsigned int &dest, const std::string &string);

void from_string(unsigned long int &dest, const std::string &string);

void from_string(unsigned long long int &dest, const std::string &string);

}
}

#endif //ICARUSFRAMEWORK_DATACONVERSION_H
