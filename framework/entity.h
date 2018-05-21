#pragma once

#include <memory>
#include <unordered_map>

#include "component.h"

class Scene;

class Entity
{
public:
	Entity(std::shared_ptr<Entity> & self);

private:
	std::weak_ptr<Entity> parent_;
	std::shared_ptr<Entity> & self_;

public:
	const std::weak_ptr<Entity> & parent(void) const;
	const std::shared_ptr<Entity> & self(void) const;

	void set_parent(const std::shared_ptr<Entity> & parent);

public:
	void Destroy(void);

private:
	std::unordered_map<std::string, std::shared_ptr<Component>> components_;

public:
	template<class _Component> const std::shared_ptr<_Component> component(void)
	{
		return std::static_pointer_cast<_Component>(this->components_[typeid(_Component).name()]);
	}
	template<class _Component, class ... Args> const std::shared_ptr<_Component> add_component(Args &&... args)
	{
		auto & target = this->components_[typeid(_Component).name()];
		target = std::make_shared<_Component>(target, args ...);
		target->set_entity(this->self_);
		target->OnAdd();
		return std::static_pointer_cast<_Component>(target);
	}

private:
	std::vector<std::shared_ptr<Entity>> children_;

public:
	template<class _Entity, class ... Args> const std::shared_ptr<_Entity> Create(Args &&... args)
	{
		this->children_.emplace_back(nullptr);
		auto & entity = this->children_.back();
		entity = std::make_shared<_Entity>(entity, args ...);
		entity->set_parent(this->self_);
		entity->Initialize();
		return std::static_pointer_cast<_Entity>(entity);
	}

public:
	virtual void Initialize(void) = 0;
	virtual void Finalize(void) = 0;

public:
	void Pause(void);
	void Update(void);
	void Always(void);
};