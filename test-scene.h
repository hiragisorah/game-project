#pragma once

#include "framework\scene.h"
#include "test-system.h"
#include "test-entity.h"

class TestScene : public Scene
{
private:
	void Initialize(void) override
	{
		this->add_system<TestSystem>();
		this->Create<TestEntity>();
	}
	void Finalize(void) override {}
};