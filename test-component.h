#pragma once

#include <iostream>

#include "framework\component.h"
#include "graphics\renderer.h"
#include "graphics\graphics.h"

class TestComponent : public Component
{
private:
	std::shared_ptr<Renderer> renderer_;

public:
	TestComponent(std::shared_ptr<Component> & self) : Component(self)
	{
		
	}

private:
	void OnAdd(void) override
	{
		this->renderer_ = std::make_shared<Renderer>();

		Graphics::Get().AddRenderer(this->renderer_);
	}

	void Update(void) override
	{

	}
};