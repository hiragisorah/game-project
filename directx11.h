#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d11.h>
#include <wrl\client.h>
#include <DirectXMath.h>

#include "graphics\graphics.h"
#include "graphics\window.h"

#pragma comment(lib, "d3d11.lib")

using namespace Microsoft::WRL;

class DirectX11 : public Graphics
{
public:
	DirectX11(const Window * window);

private:
	const Window * window_;
	
	ComPtr<ID3D11Device> device_;
	ComPtr<ID3D11DeviceContext> context_;
	ComPtr<IDXGISwapChain> swap_chain_;

private:
	ComPtr<ID3D11RenderTargetView> back_buffer_rtv_;

	ComPtr<ID3D11DepthStencilView> dsv_;
	
	D3D11_VIEWPORT viewport_;

private:
	FRAME_RATE frame_rate_;

public:
	void Initialize(void) override;
	void Finalize(void) override;
	void Clear(void) override;
	void Present(void) override;

public:
	void Rendering(const std::weak_ptr<Renderer> & renderer) override;

public:
	void CreateBackBuffer(void);
	void CreateDepthStencilView(void);
	void CreateViewPort(void);

public:
	void SetupBackBuffer(void) override;
	void SetupDeffered(void) override;
	void SetupShadowMap(void) override;
};