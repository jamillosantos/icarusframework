/*
 * @author Jamillo Santos <jamillo@gmail.com>
 * @date August 18, 2015
 */

#ifndef ICARUSFRAMEWORK_HTTP_HEADERS_H
#define ICARUSFRAMEWORK_HTTP_HEADERS_H

#include <string>
#include <map>
#include <vector>
#include <boost/optional.hpp>

namespace icarus
{
namespace http
{

class header_value
{
public:
	typedef std::vector<std::string> list;

	typedef list::iterator iterator;
	typedef list::const_iterator const_iterator;
	typedef list::reverse_iterator reverse_iterator;
	typedef list::const_reverse_iterator const_reverse_iterator;
private:
	list _values;
public:
	header_value();

	header_value(const std::string &value);

	const std::string &value();

	icarus::http::header_value &value(const std::string &value);

	icarus::http::header_value &emplace(const std::string &value);

	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;

	const_iterator cbegin() const;
	const_iterator cend() const;

	reverse_iterator rbegin();
	reverse_iterator rend();

	const_reverse_iterator crbegin() const;
	const_reverse_iterator crend() const;
};

class headers
{
public:
	typedef std::map<std::string, icarus::http::header_value> list;

	typedef std::pair<const std::string, icarus::http::header_value> pair;

	typedef list::iterator iterator;
	typedef list::const_iterator const_iterator;
	typedef list::reverse_iterator reverse_iterator;
	typedef list::const_reverse_iterator const_reverse_iterator;
private:
	list _list;
public:
	headers();

	boost::optional<icarus::http::header_value&> operator[](const std::string &name);

	iterator begin();
	const_iterator begin() const;
	iterator end();
	const_iterator end() const;

	const_iterator cbegin() const;
	const_iterator cend() const;

	reverse_iterator rbegin();
	reverse_iterator rend();

	const_reverse_iterator crbegin() const;
	const_reverse_iterator crend() const;

	void emplace(const std::string &name, const std::string &value);
};
}
}

#endif //ICARUSFRAMEWORK_HTTP_HEADERS_H
