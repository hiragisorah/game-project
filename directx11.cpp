#include "directx11.h"

DirectX11::DirectX11(const Window * window)
	: window_(window)
	, frame_rate_(FRAME_RATE_LIMITED)
{
}

void DirectX11::Initialize(void)
{
	// デバイスとスワップチェーンの作成
	DXGI_SWAP_CHAIN_DESC sd;
	memset(&sd, 0, sizeof(sd));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = this->window_->width();
	sd.BufferDesc.Height = this->window_->height();
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = this->window_->hwnd();
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = true;

	D3D_FEATURE_LEVEL feature_levels = D3D_FEATURE_LEVEL_11_0;
	D3D_FEATURE_LEVEL * feature_level = nullptr;

	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		0, &feature_levels, 1, D3D11_SDK_VERSION, &sd, &this->swap_chain_, &this->device_,
		feature_level, &this->context_);

	this->CreateBackBuffer();
	this->CreateDepthStencilView();
	this->CreateViewPort();
}

void DirectX11::Finalize(void)
{
}

void DirectX11::Clear(void)
{
	DirectX::XMVECTOR clear_color = { .2f, .4f, .8f, 1.f};
	this->context_->ClearRenderTargetView(this->back_buffer_rtv_.Get(), (float*)&clear_color);
	this->context_->ClearDepthStencilView(this->dsv_.Get(), D3D11_CLEAR_DEPTH, 1.f, 0);
}

void DirectX11::Present(void)
{
	this->swap_chain_->Present(this->frame_rate_, 0);
}

void DirectX11::Rendering(const std::weak_ptr<Renderer>& renderer)
{
}

void DirectX11::CreateBackBuffer(void)
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex_2d;

	// バックバッファーテクスチャーを取得
	this->swap_chain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&tex_2d);

	// そのテクスチャーに対しレンダーターゲットビュー(RTV)を作成
	this->device_->CreateRenderTargetView(tex_2d.Get(), nullptr, this->back_buffer_rtv_.GetAddressOf());
}

void DirectX11::CreateDepthStencilView(void)
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> tex_2d;

	//深度マップテクスチャをレンダーターゲットにする際のデプスステンシルビュー用のテクスチャーを作成
	D3D11_TEXTURE2D_DESC tex_desc = {};
	tex_desc.Width = this->window_->width();
	tex_desc.Height = this->window_->height();
	tex_desc.MipLevels = 1;
	tex_desc.ArraySize = 1;
	tex_desc.Format = DXGI_FORMAT_D32_FLOAT;
	tex_desc.SampleDesc.Count = 1;
	tex_desc.SampleDesc.Quality = 0;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tex_desc.CPUAccessFlags = 0;
	tex_desc.MiscFlags = 0;

	this->device_->CreateTexture2D(&tex_desc, nullptr, tex_2d.GetAddressOf());
	this->device_->CreateDepthStencilView(tex_2d.Get(), nullptr, this->dsv_.GetAddressOf());
}

void DirectX11::CreateViewPort(void)
{
	auto & vp = this->viewport_;

	vp.Width = this->window_->width<float>();
	vp.Height = this->window_->height<float>();
	vp.MinDepth = 0.f;
	vp.MaxDepth = 1.f;
	vp.TopLeftX = 0.f;
	vp.TopLeftY = 0.f;
}

void DirectX11::SetupBackBuffer(void)
{
	this->context_->OMSetRenderTargets(1, this->back_buffer_rtv_.GetAddressOf(), this->dsv_.Get());
	this->context_->RSSetViewports(1, &this->viewport_);
}

void DirectX11::SetupDeffered(void)
{
}

void DirectX11::SetupShadowMap(void)
{
}
