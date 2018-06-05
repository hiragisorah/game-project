#pragma once

#include <iostream>

#include "framework\component.h"
#include "graphics\renderer.h"
#include "graphics\graphics.h"

class TestComponent : public Component
{
	struct InstantBuffer
	{
		DirectX::XMMATRIX world_;
		DirectX::XMMATRIX view_;
		DirectX::XMMATRIX projection_;
	};
private:
	std::shared_ptr<Renderer> renderer_;
	InstantBuffer cbuffer_;

public:
	TestComponent(std::shared_ptr<Component> & self) : Component(self)
	{
		
	}

private:
	void OnAdd(void) override
	{
		this->renderer_ = std::make_shared<Renderer>();

		this->cbuffer_.world_ = DirectX::XMMatrixIdentity();
		this->cbuffer_.view_ = DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(0, 7.f, -7.f, 0), DirectX::XMVectorZero(), DirectX::XMVectorSet(0.f, 1.f, 0.f, 0.f));
		this->cbuffer_.projection_ = DirectX::XMMatrixPerspectiveFovLH(DirectX::XM_PIDIV4, 1920.f / 720.f, 0.1f, 1000.f);

		this->renderer_->constant_buffer_ = &this->cbuffer_;

		Graphics::Get().AddRenderer(this->renderer_);
	}

	void Update(void) override
	{
		static float y = 0.f;

		y += .03f;

		this->cbuffer_.world_ = DirectX::XMMatrixScaling(0.1f, 0.1f, 0.1f) * DirectX::XMMatrixRotationY(y);
	}
};