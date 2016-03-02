/**
 * @author J. Santos <jamillo@gmail.com>
 * @date March 02, 2016
 **/

#ifndef ICARUSFRAMEWORK_DB_MIGRATIONS_HPP
#define ICARUSFRAMEWORK_DB_MIGRATIONS_HPP

#include <soci/session.h>

namespace icarus
{
namespace db
{
class migration
{
public:
	virtual void up(soci::session &session) = 0;

	virtual void down(soci::session &session) = 0;
};

// TODO: Migration manager
}
}

#endif //ICARUSFRAMEWORK_DB_MIGRATIONS_HPP
