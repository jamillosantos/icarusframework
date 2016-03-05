/**
 * @author J. Santos <jamillo@gmail.com>
 * @date March 03, 2016
 **/

#include "icarus/migrations/migration.h"

#include <boost/regex.hpp>

namespace bl = boost::locale;

icarus::migrations::invalid_version::invalid_version(const std::string &version)
	: icarus::exception((bl::format(bl::translate("Version '{1}' is invalid.")) % version).str())
{ }

icarus::migrations::version &icarus::migrations::version::operator=(const std::string &version)
{
	boost::regex r("^([^.]+)(.([^.]+))?(.([^.]+))?(.([^.]+))?$");
	boost::smatch match;
	if (boost::regex_match(version, match, r))
	{
		if (match.size() > 0)
			this->major = match[1];
		if (match.size() > 2)
			this->minor = match[3];
		if (match.size() > 4)
			this->patch = match[5];
		if (match.size() > 6)
			this->tweak = match[7];

		this->value = version;
	}
	else
		throw icarus::migrations::invalid_version(version);
	return *this;
}

icarus::migrations::version::version()
{ }

icarus::migrations::version::version(const std::string &version)
{
	(*this) = version;
}

icarus::migrations::version::version(const icarus::migrations::version &version)
{
	(*this) = version.value;
}

bool icarus::migrations::version::operator>(const icarus::migrations::version &version)
{
	return
		(this->major > version.major)
		|| (this->minor > version.minor)
		|| (this->patch > version.patch)
		|| (this->tweak > version.tweak);
}

bool icarus::migrations::version::operator<(const icarus::migrations::version &version)
{
	return
		(this->major < version.major)
		|| (this->minor < version.minor)
		|| (this->patch < version.patch)
		|| (this->tweak < version.tweak);
}

bool icarus::migrations::version::operator==(const icarus::migrations::version &version)
{
	return
		(this->major == version.major)
		&& (this->minor == version.minor)
		&& (this->patch == version.patch)
		&& (this->tweak == version.tweak);
}


icarus::migrations::group::group(const std::string &version)
	: _version(version)
{ }

void icarus::migrations::group::add(icarus::migrations::migration *migration)
{
	migration->id(this->_migrations.size()+1);
	this->_migrations.emplace_back(migration);
}

icarus::migrations::group::iterator icarus::migrations::group::begin()
{
	return this->_migrations.begin();
}

icarus::migrations::group::iterator icarus::migrations::group::end()
{
	return this->_migrations.end();
}

const icarus::migrations::version &icarus::migrations::group::version()
{
	return this->_version;
}

unsigned int icarus::migrations::group::size()
{
	return this->_migrations.size();
}

bool icarus::migrations::group::empty()
{
	return this->_migrations.empty();
}

icarus::migrations::group::reverse_iterator icarus::migrations::group::rbegin()
{
	return this->_migrations.rbegin();
}

icarus::migrations::group::reverse_iterator icarus::migrations::group::rend()
{
	return this->_migrations.rend();
}

icarus::migrations::migration_id_t icarus::migrations::migration::id()
{
	return this->_id;
}

void icarus::migrations::migration::id(icarus::migrations::migration_id_t id)
{
	this->_id = id;
}


