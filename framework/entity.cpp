#include "entity.h"
#include "entity.h"
#include "entity.h"
#include "entity.h"
#include "entity.h"
#include "entity.h"
#include "entity.h"
#include "entity.h"

Entity::Entity(std::shared_ptr<Entity>& self)
	: self_(self)
{
}

const std::weak_ptr<Entity>& Entity::parent(void) const
{
	return this->parent_;
}

const std::shared_ptr<Entity>& Entity::self(void) const
{
	return this->self_;
}

void Entity::set_parent(const std::shared_ptr<Entity>& parent)
{
	this->parent_ = parent;
}

void Entity::Destroy(void)
{
	this->Finalize();
	this->self_.reset();
}

void Entity::Pause(void)
{
	for (auto && component : this->components_)
		if (component.second)
		{
			component.second->Pause();
		}
		else
		{
			this->components_.erase(component.first);
		}

	for (unsigned int n = 0; n < this->children_.size(); ++n)
		if (this->children_[n])
		{
			this->children_[n]->Pause();
		}
		else
		{
			this->children_.erase(this->children_.begin() + n);
		}
}

void Entity::Update(void)
{
	for (auto && component : this->components_)
		if (component.second)
		{
			component.second->Update();
		}
		else
		{
			this->components_.erase(component.first);
		}

	for (unsigned int n = 0; n < this->children_.size(); ++n)
		if (this->children_[n])
		{
			this->children_[n]->Update();
		}
		else
		{
			this->children_.erase(this->children_.begin() + n);
		}
}

void Entity::Always(void)
{
	for (auto && component : this->components_)
		if (component.second)
		{
			component.second->Always();
		}
		else
		{
			this->components_.erase(component.first);
		}

	for (unsigned int n = 0; n < this->children_.size(); ++n)
		if (this->children_[n])
		{
			this->children_[n]->Always();
		}
		else
		{
			this->children_.erase(this->children_.begin() + n);
		}
}
