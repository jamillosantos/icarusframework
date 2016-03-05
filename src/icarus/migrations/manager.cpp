/**
 * @author J. Santos <jamillo@gmail.com>
 * @date March 02, 2016
 **/

#include <icarus/migrations/manager.h>

#include <icarus/log.h>

icarus::migrations::manager::identification::identification()
{ }

icarus::migrations::manager::identification::identification(const icarus::migrations::manager::iterator &group, const icarus::migrations::group::iterator &migration)
	: group(group), migration(migration)
{ }

void icarus::migrations::manager::upgrade()
{
	this->ensureSchema();
	this->fetch();

	if (!this->_migrations.empty())
	{
		icarus::migrations::manager::reverse_iterator last = this->_migrations.rbegin();
		while (last->second->empty())
			++last;
		if (last != this->_migrations.rend())
			this->to(last->first, (*last->second->rbegin())->id());
		// else, nothing to do
	}
	// else, nothing to do
}

void icarus::migrations::manager::add(icarus::migrations::migration *migration)
{
	const std::string &version = migration->version();
	auto it = this->_migrations.find(version);

	if (it == this->_migrations.end())
		this->_migrations.emplace(std::make_pair(version, std::unique_ptr<icarus::migrations::group>(new icarus::migrations::group(version))));
	this->_migrations[version]->add(migration);
}

icarus::migrations::manager::identification icarus::migrations::manager::current()
{
	if (this->_data.empty())
	{
		// If empty, start from the beginning
		auto item = this->_migrations.begin();
		return icarus::migrations::manager::identification(item, (*item->second).begin());
	}
	else
	{
		auto last = (this->_data.rbegin());

		icarus::migrations::manager::iterator group = this->_migrations.find(last->version.value);
		if (group == this->_migrations.end())
			// TODO not found
			throw icarus::exception("Cannot find the specified version.");
		else
		{
			if (group->second->size() >= last->id)
				return icarus::migrations::manager::identification(group, (group->second->begin() += (last->id - 1)));
			else
				// TODO
				throw icarus::exception((bl::format(bl::translate("Migration ID '{1}' greater than defined ({2}).")) % last->id % std::to_string(group->second->size())).str());
		}
	}
}

void icarus::migrations::manager::to(const std::string &version, icarus::migrations::migration_id_t id)
{
	this->ensureSchema();
	this->fetch();

	const icarus::migrations::manager::identification &c = this->current();
	icarus::migrations::version v(version);
	boost::optional<bool> forward;
	if (v == c.group->second->version())
	{
		if (id < (*c.migration)->id())
			forward = false;
		else if (id >= (*c.migration)->id())
			forward = true;
	}
	else
		forward = (v > c.group->second->version());

	if (forward && (*forward)) // foward set and foward value true
	{
		icarus::migrations::group::iterator migration_it(c.migration);

		if (!this->_data.empty())
			++migration_it;

		for (icarus::migrations::manager::iterator group_it = c.group; group_it != this->_migrations.end(); ++group_it, migration_it = (*group_it->second).begin())
		{
			LOG_INFO("Starting version: " << group_it->first);
			for (; migration_it != (*group_it->second).end(); ++migration_it)
			{
				LOG_INFO("Running migration " << (*migration_it)->id() << " (up) ...");
				(*migration_it)->up();
				this->insert(*(*migration_it));
				LOG_INFO("OK");
			}
		}
	}
	else if (forward) // foward set (implicit foward value false)
	{
		icarus::migrations::group::reverse_iterator migration_it(c.migration);
		for (icarus::migrations::manager::reverse_iterator group_it(c.group); group_it != this->_migrations.rend(); ++group_it, migration_it = (*group_it->second).rbegin())
		{
			LOG_INFO("Starting version: " << group_it->first);
			for (; migration_it != (*group_it->second).rend(); ++migration_it)
			{
				LOG_INFO("Running migration " << (*migration_it)->id() << " (down) ...");
				(*migration_it)->down();
				this->remove(*(*migration_it));
				LOG_INFO("OK");
			}
		}
	}
	// else nothing to do.
}

void icarus::migrations::manager::to(const std::string &version)
{
	this->ensureSchema();
	this->fetch();

	icarus::migrations::manager::iterator it = this->_migrations.find(version);
	if (it == this->_migrations.end())
		// TODO
		throw icarus::exception("");
	else
	{
		if (!(*it->second).empty())
			this->to(version, (*((*it->second).rend()))->id());
	}
}

icarus::migrations::manager::manager()
	: _fetched(false)
{ }

void icarus::migrations::manager::ensureSchema()
{
	if (this->schemaExists())
	{
		LOG_INFO("Schema exists");
	}
	else
		this->createSchema();
}

