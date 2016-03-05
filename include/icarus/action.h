/**
 * @author J. Santos <jamillo@gmail.com>
 * @date November 11, 2015
 **/

#ifndef ICARUSFRAMEWORK_ACTION_H
#define ICARUSFRAMEWORK_ACTION_H

#include <string>

namespace icarus
{
class action
{
private:
	std::string _method;
	std::string _url;
public:
	action(const action &action);

	action(std::string url);

	action(std::string method, std::string url);

	const std::string &method() const;

	action &method(std::string method);

	const std::string &url() const;

	action &url(std::string url);
};
}


#endif //ICARUSFRAMEWORK_ACTION_H
