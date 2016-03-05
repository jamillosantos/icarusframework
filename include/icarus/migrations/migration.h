/**
 * @author J. Santos <jamillo@gmail.com>
 * @date March 03, 2016
 **/

#ifndef ICARUSFRAMEWORK_MIGRATIONS_MIGRATION_H
#define ICARUSFRAMEWORK_MIGRATIONS_MIGRATION_H

#include <soci/session.h>

#include <icarus/exceptions.h>

namespace icarus
{
namespace migrations
{
typedef unsigned int migration_id_t;

class invalid_version
	: public icarus::exception
{
public:
	invalid_version(const std::string &version);
};

struct version
{
	version();

	version(const std::string &version);

	version(const icarus::migrations::version &version);

	std::string value;

	std::string major;
	std::string minor;
	std::string patch;
	std::string tweak;

	icarus::migrations::version &operator=(const std::string &version);

	bool operator>(const icarus::migrations::version &version);

	bool operator<(const icarus::migrations::version &version);

	bool operator==(const icarus::migrations::version &version);
};

/**
 * @interface
 */
class migration
{
private:
	icarus::migrations::migration_id_t _id;
public:
	virtual const std::string version() = 0;

	icarus::migrations::migration_id_t id();

	void id(icarus::migrations::migration_id_t id);

	virtual void up() = 0;

	virtual void down() = 0;
};

class group
{
public:
	typedef typename std::vector<std::unique_ptr<icarus::migrations::migration>>::iterator iterator;
	typedef typename std::vector<std::unique_ptr<icarus::migrations::migration>>::reverse_iterator reverse_iterator;
private:
	std::vector<std::unique_ptr<migration>> _migrations;

	icarus::migrations::version _version;
public:
	group(const std::string &version);

	void add(icarus::migrations::migration *migration);

	const icarus::migrations::version &version();

	unsigned int size();

	bool empty();

	iterator begin();

	iterator end();

	reverse_iterator rbegin();

	reverse_iterator rend();
};
}
}

#endif //ICARUSFRAMEWORK_MIGRATIONS_MIGRATION_H
