#pragma once

#include "framework\entity.h"
#include "test-component.h"

class TestEntity : public Entity
{
public:
	TestEntity(std::shared_ptr<Entity> & self) : Entity(self) {}

private:
	void Initialize(void) override
	{
		this->add_component<TestComponent>();
	}
	void Finalize(void) override {}
};