#pragma once

#include <iostream>

#include "framework\system.h"

class TestSystem : public System
{
public:
	TestSystem(std::shared_ptr<System> & self) : System(self) {}
};