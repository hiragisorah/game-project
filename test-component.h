#pragma once

#include <iostream>

#include "framework\component.h"

class TestComponent : public Component
{
public:
	TestComponent(std::shared_ptr<Component> & self) : Component(self)
	{

	}

private:
	void OnAdd(void) override
	{

	}

	void Update(void) override
	{

	}
};