#pragma once

#include <wrl\client.h>
#include <d3d11.h>
#include <d3dcompiler.h>

#include <iostream>
#include <vector>

#include "resource-manager\resource-manager.h"

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "DirectXTex.lib")

using namespace Microsoft::WRL;

struct Dx11Shader : public Shader
{
	std::vector<ComPtr<ID3D11Buffer>> constant_buffer_;
	ComPtr<ID3D11VertexShader> vertex_shader_;
	ComPtr<ID3D11GeometryShader> geometry_shader_;
	ComPtr<ID3D11PixelShader> pixel_shader_;
	ComPtr<ID3D11InputLayout> input_layout_;
};

struct Dx11Texture : public Texture
{
	ComPtr<ID3D11ShaderResourceView> srv_;
	ComPtr<ID3D11Buffer> vertex_buffer_;
};

struct Dx11Model : public Model
{
	struct Mesh
	{
		ComPtr<ID3D11ShaderResourceView> srv_;
		ComPtr<ID3D11Buffer> vertex_buffer_;
		ComPtr<ID3D11Buffer> index_buffer_;
		unsigned int index_cnt_;

		Mesh(void)
			: index_cnt_(0U)
		{}
	};

	std::vector<Mesh> meshes_;
};

class Dx11ResourceManager : public ResourceManager
{
public:
	void LoadShader(const Resource::Shader::PATH & path, std::shared_ptr<Shader> & shader) override;
	void LoadTexture(const Resource::Texture::PATH & path, std::shared_ptr<Texture> & texture) override;
	void LoadModel(const Resource::Model::PATH & path, std::shared_ptr<Model> & model) override;
	
private:
	void CreateInputLayoutAndConstantBufferFromShader(const std::shared_ptr<Dx11Shader> & shader, ID3DBlob * blob);
	DXGI_FORMAT GetDxgiFormat(D3D_REGISTER_COMPONENT_TYPE type, BYTE mask);
	template<class _Vertex> void CreateVertexBuffer(Dx11Model::Mesh & mesh, std::vector<_Vertex> & vertices)
	{
		D3D11_BUFFER_DESC bd = {};
		bd.ByteWidth = vertices.size() * sizeof(_Vertex);
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = vertices.data();

		Graphics::Get<DirectX11>()->device()->CreateBuffer(&bd, &sd, mesh.vertex_buffer_.GetAddressOf());
	}
	template<class _Index> void CreateIndexBuffer(Dx11Model::Mesh & mesh, std::vector<_Index> & indices)
	{
		mesh.index_cnt_ = indices.size();

		D3D11_BUFFER_DESC bd = {};
		bd.ByteWidth = mesh.index_cnt_ * sizeof(_Index);
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
		bd.Usage = D3D11_USAGE_DEFAULT;

		D3D11_SUBRESOURCE_DATA sd = {};
		sd.pSysMem = indices.data();

		Graphics::Get<DirectX11>()->device()->CreateBuffer(&bd, &sd, mesh.index_buffer_.GetAddressOf());
	}
};