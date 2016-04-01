/**
 * @author J. Santos <jamillo@gmail.com>
 * @date March 12, 2016
 **/

#ifndef ICARUSFRAMEWORK_ROUTES_TYPECONVERSION_COMMONS_H
#define ICARUSFRAMEWORK_ROUTES_TYPECONVERSION_COMMONS_H

#include <string>
#include <json/value.h>
#include <json/writer.h>
#include <json/reader.h>

namespace icarus
{
template<>
struct type_conversion<unsigned int>
{
	typedef std::string base_type;

	static void from(base_type const &in, unsigned int &out)
	{
		out = std::stoi(in);
	}

	static void to(unsigned int const &in, base_type &out)
	{
		out = std::to_string(in);
	}
};

template<>
struct type_conversion<int>
{
	typedef std::string base_type;

	static void from(base_type const &in, int &out)
	{
		out = std::stoi(in);
	}

	static void to(int const &in, base_type &out)
	{
		out = std::to_string(in);
	}
};

template<>
struct type_conversion<unsigned short>
{
	typedef std::string base_type;

	static void from(base_type const &in, unsigned short &out)
	{
		out = std::stoi(in);
	}

	static void to(unsigned short const &in, base_type &out)
	{
		out = std::to_string(in);
	}
};

template<>
struct type_conversion<short>
{
	typedef std::string base_type;

	static void from(base_type const &in, short &out)
	{
		out = std::stoi(in);
	}

	static void to(short const &in, base_type &out)
	{
		out = std::to_string(in);
	}
};

template<>
struct type_conversion<unsigned long>
{
	typedef std::string base_type;

	static void from(base_type const &in, unsigned long &out)
	{
		out = std::stoi(in);
	}

	static void to(unsigned long const &in, base_type &out)
	{
		out = std::to_string(in);
	}
};

template<>
struct type_conversion<long>
{
	typedef std::string base_type;

	static void from(base_type const &in, long &out)
	{
		out = std::stoi(in);
	}

	static void to(long const &in, base_type &out)
	{
		out = std::to_string(in);
	}
};

template<>
struct type_conversion<unsigned long long>
{
	typedef std::string base_type;

	static void from(base_type const &in, unsigned long long &out)
	{
		out = std::stoi(in);
	}

	static void to(unsigned long long const &in, base_type &out)
	{
		out = std::to_string(in);
	}
};

template<>
struct type_conversion<long long>
{
	typedef std::string base_type;

	static void from(base_type const &in, long long &out)
	{
		out = std::stoi(in);
	}

	static void to(long long const &in, base_type &out)
	{
		out = std::to_string(in);
	}
};

template<>
struct type_conversion<float>
{
	typedef std::string base_type;

	static void from(base_type const &in, float &out)
	{
		out = std::stof(in);
	}

	static void to(float const &in, base_type &out)
	{
		out = std::to_string(in);
	}
};

template<>
struct type_conversion<double>
{
	typedef std::string base_type;

	static void from(base_type const &in, double &out)
	{
		out = std::stod(in);
	}

	static void to(double const &in, base_type &out)
	{
		out = std::to_string(in);
	}
};

template<>
struct type_conversion<bool>
{
	typedef std::string base_type;

	static void from(base_type const &in, bool &out)
	{
		out = ((in == "1") || (in == "on"));
	}

	static void to(bool const &in, base_type &out)
	{
		out = (in ? "on" : "off");
	}
};

template<>
struct type_conversion<Json::Value>
{
	typedef std::string base_type;

	static void from(base_type const &in, Json::Value &out)
	{
		Json::Reader reader;
		reader.parse(in, out, false);
	}

	static void to(Json::Value const &in, base_type &out)
	{
		Json::FastWriter writer;
		out = writer.write(in);
	}
};
}

#endif //ICARUSFRAMEWORK_ROUTES_TYPECONVERSION_COMMONS_H
