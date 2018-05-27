#include "directx11.h"

#include <fstream>

#include "dx11-resource-manager.h"
#include "graphics\graphics.h"
#include "DirectXTK\WICTextureLoader.h"

#if defined(DEBUG) || defined(_DEBUG)
constexpr DWORD SHADER_FLAGS = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG;
#else
constexpr DWORD SHADER_FLAGS = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif

void Dx11ResourceManager::LoadShader(const Resource::Shader::PATH & path, std::shared_ptr<Shader> & shader)
{
	auto s = std::make_shared<Dx11Shader>();
	shader = s;

	std::string p = Resource::Shader::paths[(unsigned int)path];

	ID3DBlob * blob = nullptr;
	ID3DBlob * error = nullptr;

	auto graphics = Graphics::Get<DirectX11>();

	if (FAILED(D3DCompileFromFile(std::wstring(p.begin(), p.end()).c_str(), nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, "VS", "vs_5_0", SHADER_FLAGS, 0, &blob, &error)))
	{
		if (error != nullptr)
			std::cout << __FUNCTION__ << " " << (char*)error->GetBufferPointer() << std::endl;
		else
			std::cout << __FUNCTION__ << " シェーダーの読み込みに失敗しました。" << std::endl;

		return;
	}
	else
	{
		graphics->device()->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, s->vertex_shader_.GetAddressOf());
		this->CreateInputLayoutAndConstantBufferFromShader(s, blob);
	}

	if (SUCCEEDED(D3DCompileFromFile(std::wstring(p.begin(), p.end()).c_str(), nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, "GS", "gs_5_0", SHADER_FLAGS, 0, &blob, &error)))
	{
		graphics->device()->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, s->geometry_shader_.GetAddressOf());
	}

	if (SUCCEEDED(D3DCompileFromFile(std::wstring(p.begin(), p.end()).c_str(), nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, "PS", "ps_5_0", SHADER_FLAGS, 0, &blob, &error)))
	{
		graphics->device()->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, s->pixel_shader_.GetAddressOf());
	}
}

void CreateTexture(const std::shared_ptr<Dx11Texture> & texture, DirectX11 * graphics, const Resource::Texture::PATH & path)
{
	ComPtr<ID3D11Resource> res;

	std::string p = Resource::Texture::paths[(unsigned int)path];

	std::wstring w_file_path = std::wstring(p.begin(), p.end());
	DirectX::CreateWICTextureFromFile(graphics->device().Get(), w_file_path.c_str(), res.GetAddressOf(), texture->srv_.GetAddressOf());

	ComPtr<ID3D11Texture2D> tex;
	res.As(&tex);
	D3D11_TEXTURE2D_DESC desc;

	tex->GetDesc(&desc);

	texture->size_.x = static_cast<float>(desc.Width);
	texture->size_.y = static_cast<float>(desc.Height);
}

void CreateVertexBufferFromTextureSize(const std::shared_ptr<Dx11Texture> & texture, DirectX11 * graphics)
{
	struct SimpleVertex
	{
		DirectX::XMFLOAT3 position_;
		DirectX::XMFLOAT2 texcoord_;
	};

	auto & x = texture->size_.x;
	auto & y = texture->size_.y;

	SimpleVertex vertices[] =
	{
		DirectX::XMFLOAT3(-x / 2.f,-y / 2.f,0),DirectX::XMFLOAT2(0,0),//頂点1,
		DirectX::XMFLOAT3(+x / 2.f,-y / 2.f,0), DirectX::XMFLOAT2(1,0),//頂点2
		DirectX::XMFLOAT3(-x / 2.f,+y / 2.f,0),DirectX::XMFLOAT2(0,1), //頂点3
		DirectX::XMFLOAT3(+x / 2.f,+y / 2.f,0),DirectX::XMFLOAT2(1,1), //頂点4
	};

	D3D11_BUFFER_DESC bd;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = sizeof(SimpleVertex) * (sizeof(vertices) / sizeof(vertices[0]));
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices;

	graphics->device()->CreateBuffer(&bd, &InitData, texture->vertex_buffer_.GetAddressOf());
}

void Dx11ResourceManager::LoadTexture(const Resource::Texture::PATH & path, std::shared_ptr<Texture>& texture)
{
	auto graphics = Graphics::Get<DirectX11>();

	auto dx_texture = std::make_shared<Dx11Texture>();
	texture = dx_texture;

	CreateTexture(dx_texture, graphics, path);
	CreateVertexBufferFromTextureSize(dx_texture, graphics);
}

void Dx11ResourceManager::LoadModel(const Resource::Model::PATH & path, std::shared_ptr<Model>& model)
{
	auto graphics = Graphics::Get<DirectX11>();

	auto dx_model = std::make_shared<Dx11Model>();
	model = dx_model;

	std::string file_path = Resource::Model::paths[(unsigned int)path];

	struct Vertex
	{
		DirectX::XMFLOAT3 position_;
		DirectX::XMFLOAT3 normal_;
		DirectX::XMFLOAT2 uv_;
	};

	struct Mesh
	{
		std::string texture_;
		std::vector<Vertex> vertices_;
		std::vector<unsigned int> indices_;
	};

	std::vector<Mesh> meshes;

	meshes.clear();

	std::ifstream file;

	file.open(file_path, std::ios::in | std::ios::binary);

	if (file.fail())
	{
		std::cout << "モデルファイルの読み込みに失敗しました。" << std::endl;
		return;
	}

	unsigned int mesh_cnt;

	file.read(reinterpret_cast<char*>(&mesh_cnt), sizeof(unsigned int));

	meshes.resize(mesh_cnt);
	dx_model->meshes_.resize(mesh_cnt);

	for (unsigned int m = 0; m < mesh_cnt; ++m)
	{
		auto & mesh = meshes[m];

		unsigned int vtx_cnt = 0;

		file.read(reinterpret_cast<char*>(&vtx_cnt), sizeof(unsigned int));

		mesh.vertices_.resize(vtx_cnt);

		for (unsigned int v = 0; v < vtx_cnt; ++v)
		{
			auto & vtx = mesh.vertices_[v];

			file.read(reinterpret_cast<char*>(&vtx), sizeof(Vertex));
		}

		unsigned int index_cnt = 0;

		file.read(reinterpret_cast<char*>(&index_cnt), sizeof(unsigned int));

		mesh.indices_.resize(index_cnt);

		for (unsigned int i = 0; i < index_cnt; ++i)
		{
			auto & index = mesh.indices_[i];

			file.read(reinterpret_cast<char*>(&index), sizeof(unsigned int));
		}

		unsigned texture_str_cnt = 0;

		file.read(reinterpret_cast<char*>(&texture_str_cnt), sizeof(unsigned int));

		mesh.texture_.resize(texture_str_cnt);

		if (texture_str_cnt > 0)
		{
			char * texture_str = new char[texture_str_cnt + 1];
			file.read(&texture_str[0], sizeof(char) * texture_str_cnt);
			texture_str[texture_str_cnt] = '\0';
			mesh.texture_ = texture_str;
			if (mesh.texture_.find(".") < mesh.texture_.size())
				mesh.texture_ = mesh.texture_.substr(mesh.texture_.rfind("\\") + 1, mesh.texture_.size() - mesh.texture_.rfind("\\"));
			else
				mesh.texture_ = "";

			delete[] texture_str;
		}
	}

	file.close();

	for (unsigned int n = 0; n < mesh_cnt; ++n)
	{
		if (meshes[n].texture_ != "")
		{
			auto dir = file_path;
			dir.replace(dir.rfind("/"), dir.size() - dir.rfind("/"), "/" + meshes[n].texture_);
			std::wstring w_file_path = std::wstring(dir.begin(), dir.end());
			DirectX::CreateWICTextureFromFile(graphics->device().Get(), w_file_path.c_str(), nullptr, dx_model->meshes_[n].srv_.GetAddressOf());
		}

		this->CreateVertexBuffer(dx_model->meshes_[n], meshes[n].vertices_);
		this->CreateIndexBuffer(dx_model->meshes_[n], meshes[n].indices_);
	}
}

void Dx11ResourceManager::CreateInputLayoutAndConstantBufferFromShader(const std::shared_ptr<Dx11Shader> & shader, ID3DBlob * blob)
{
	auto graphics = Graphics::Get<DirectX11>();

	ID3D11ShaderReflection * reflector = nullptr;
	D3DReflect(blob->GetBufferPointer(), blob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&reflector);

	D3D11_SHADER_DESC shader_desc;
	reflector->GetDesc(&shader_desc);

	shader->constant_buffer_.resize(shader_desc.ConstantBuffers);

	for (unsigned int n = 0; n < shader_desc.ConstantBuffers; ++n)
	{
		int size = 0;
		auto cb = reflector->GetConstantBufferByIndex(n);
		D3D11_SHADER_BUFFER_DESC desc;
		cb->GetDesc(&desc);

		for (size_t j = 0; j < desc.Variables; ++j)
		{
			auto v = cb->GetVariableByIndex(j);
			D3D11_SHADER_VARIABLE_DESC vdesc;
			v->GetDesc(&vdesc);
			if (vdesc.Size % 16)
				size += vdesc.Size + 16 - (vdesc.Size % 16);
			else
				size += vdesc.Size;
		}

		D3D11_BUFFER_DESC bd;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		bd.ByteWidth = size;
		bd.CPUAccessFlags = 0;
		bd.MiscFlags = 0;
		bd.StructureByteStride = 0;
		bd.Usage = D3D11_USAGE_DEFAULT;

		if (FAILED(graphics->device()->CreateBuffer(&bd, nullptr, shader->constant_buffer_[n].GetAddressOf())))
			std::cout << "コンスタントバッファーの作成に失敗しました。" << std::endl;
	}

	std::vector<D3D11_INPUT_ELEMENT_DESC> element;
	for (unsigned int i = 0; i < shader_desc.InputParameters; ++i) {
		D3D11_SIGNATURE_PARAMETER_DESC sigdesc;
		reflector->GetInputParameterDesc(i, &sigdesc);

		auto format = GetDxgiFormat(sigdesc.ComponentType, sigdesc.Mask);

		D3D11_INPUT_ELEMENT_DESC eledesc =
		{
			sigdesc.SemanticName
			, sigdesc.SemanticIndex
			, format
			, 0
			, D3D11_APPEND_ALIGNED_ELEMENT
			, D3D11_INPUT_PER_VERTEX_DATA
			, 0
		};

		element.emplace_back(eledesc);
	}

	if (!element.empty())
		if (FAILED(graphics->device()->CreateInputLayout(&element[0], element.size(),
			blob->GetBufferPointer(), blob->GetBufferSize(), shader->input_layout_.GetAddressOf())))
			std::cout << "インプットレイアウトの作成に失敗しました。" << std::endl;
}

DXGI_FORMAT Dx11ResourceManager::GetDxgiFormat(D3D_REGISTER_COMPONENT_TYPE type, BYTE mask)
{
	if (mask == 0x0F)
	{
		// xyzw
		switch (type)
		{
		case D3D_REGISTER_COMPONENT_FLOAT32:
			return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case D3D_REGISTER_COMPONENT_UINT32:
			return DXGI_FORMAT_R32G32B32A32_UINT;
		}
	}

	if (mask == 0x07)
	{
		// xyz
		switch (type)
		{
		case D3D_REGISTER_COMPONENT_FLOAT32:
			return DXGI_FORMAT_R32G32B32_FLOAT;
		case D3D_REGISTER_COMPONENT_UINT32:
			return DXGI_FORMAT_R32G32B32_UINT;
		}
	}

	if (mask == 0x3)
	{
		// xy
		switch (type)
		{
		case D3D_REGISTER_COMPONENT_FLOAT32:
			return DXGI_FORMAT_R32G32_FLOAT;
		case D3D_REGISTER_COMPONENT_UINT32:
			return DXGI_FORMAT_R32G32_UINT;
		}
	}

	if (mask == 0x1)
	{
		// x
		switch (type)
		{
		case D3D_REGISTER_COMPONENT_FLOAT32:
			return DXGI_FORMAT_R32_FLOAT;
		case D3D_REGISTER_COMPONENT_UINT32:
			return DXGI_FORMAT_R32_UINT;
		}
	}

	return DXGI_FORMAT_UNKNOWN;
}
