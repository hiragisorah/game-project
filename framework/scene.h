#pragma once

#include <vector>
#include <memory>

#include "entity.h"
#include "system.h"

class Scene
{
public:
	virtual void Initialize(void) = 0;
	virtual void Finalize(void) = 0;

private:
	std::unordered_map<std::string, std::shared_ptr<System>> systems_;

public:
	template<class _System> const std::shared_ptr<_System> system(void)
	{
		return std::static_pointer_cast<_System>(this->systems_[typeid(_System).name()]);
	}
	template<class _System, class ... Args> const std::shared_ptr<_System> add_system(Args &&... args)
	{
		auto & target = this->systems_[typeid(_System).name()];
		target = std::make_shared<_System>(target, args ...);
		target->OnAdd();
		return std::static_pointer_cast<_System>(target);
	}

private:
	std::vector<std::shared_ptr<Entity>> entities_;

public:
	template<class _Entity, class ... Args> const std::weak_ptr<_Entity> Create(const Args &... args)
	{
		this->entities_.emplace_back(nullptr);
		auto & entity = this->entities_.back();
		entity = std::make_shared<_Entity>(entity, args ...);
		entity->Initialize();
		return std::static_pointer_cast<_Entity>(entity);
	}

public:
	void Pause(void);
	void Update(void);
	void Always(void);
};