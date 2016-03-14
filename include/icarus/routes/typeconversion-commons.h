/**
 * @author J. Santos <jamillo@gmail.com>
 * @date March 12, 2016
 **/

#ifndef ICARUSFRAMEWORK_ROUTES_TYPECONVERSION_COMMONS_H
#define ICARUSFRAMEWORK_ROUTES_TYPECONVERSION_COMMONS_H

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
}

#endif //ICARUSFRAMEWORK_ROUTES_TYPECONVERSION_COMMONS_H
