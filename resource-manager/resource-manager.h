#pragma once

#include <memory>
#include <unordered_map>

#include "..\resource.h"

#include <DirectXMath.h>

struct Shader
{

};

struct Texture
{
	Texture(void)
		: size_(DirectX::XMFLOAT2(0, 0))
	{}

	DirectX::XMFLOAT2 size_;
};

struct Model
{

};

class ResourceManager
{
public:
	ResourceManager(void) noexcept(false);
	ResourceManager(ResourceManager && move_from) noexcept;
	ResourceManager & operator= (ResourceManager && move_from) noexcept;

	ResourceManager(ResourceManager const &) = delete;
	ResourceManager & operator=(ResourceManager const &) = delete;

	virtual ~ResourceManager(void);

private:
	std::unordered_map<Resource::Shader::PATH, std::shared_ptr<Shader>> shader_db_;
	std::unordered_map<Resource::Texture::PATH, std::shared_ptr<Texture>> texture_db_;
	std::unordered_map<Resource::Model::PATH, std::shared_ptr<Model>> model_db_;

public:
	void LoadShader(Resource::Shader::PATH path)
	{
		auto & shader = this->shader_db_[path];
		this->LoadShader(path, shader);
	}
	void UnloadShader(Resource::Shader::PATH path)
	{
		this->shader_db_.erase(path);
	}
	const std::shared_ptr<Shader> & GetShader(Resource::Shader::PATH path)
	{
		return this->shader_db_[path];
	}
	template<class _Shader> const std::shared_ptr<_Shader> GetShader(Resource::Shader::PATH path)
	{
		return std::static_pointer_cast<_Shader>(this->shader_db_[path]);
	}

public:
	void LoadTexture(Resource::Texture::PATH path)
	{
		auto & texture = this->texture_db_[path];
		this->LoadTexture(path, texture);
	}
	void UnloadTexture(Resource::Texture::PATH path)
	{
		this->texture_db_.erase(path);
	}
	const std::shared_ptr<Texture> & GetTexture(Resource::Texture::PATH path)
	{
		return this->texture_db_[path];
	}
	template<class _Texture> const std::shared_ptr<_Texture> GetTexture(Resource::Texture::PATH path)
	{
		return std::static_pointer_cast<_Texture>(this->texture_db_[path]);
	}

public:
	void LoadModel(Resource::Model::PATH path)
	{
		auto & model = this->model_db_[path];
		this->LoadModel(path, model);
	}
	void UnloadModel(Resource::Model::PATH path)
	{
		this->model_db_.erase(path);
	}
	const std::shared_ptr<Model> & GetModel(Resource::Model::PATH path)
	{
		return this->model_db_[path];
	}
	template<class _Model> const std::shared_ptr<_Model> GetModel(Resource::Model::PATH path)
	{
		return std::static_pointer_cast<_Model>(this->model_db_[path]);
	}

public:
	virtual void LoadShader(const Resource::Shader::PATH & path, std::shared_ptr<Shader> & shader) = 0;
	virtual void LoadTexture(const Resource::Texture::PATH & path, std::shared_ptr<Texture> & texture) = 0;
	virtual void LoadModel(const Resource::Model::PATH & path, std::shared_ptr<Model> & model) = 0;

public:
	// singleton
	static ResourceManager & __cdecl Get(void);
	template<class _ResourceManager> static _ResourceManager * __cdecl Get(void)
	{
		return static_cast<_ResourceManager*>(&ResourceManager::Get());
	}

private:
	class Impl;

	std::unique_ptr<Impl> impl_;
};