#pragma once

#include "framework\scene.h"
#include "test-system.h"
#include "test-entity.h"
#include "resource-manager\resource-manager.h"

class TestScene : public Scene
{
private:
	void Initialize(void) override
	{
		ResourceManager::Get().LoadShader(Resource::Shader::PATH::shader_2d);
		ResourceManager::Get().LoadShader(Resource::Shader::PATH::shader_3d);
		ResourceManager::Get().LoadTexture(Resource::Texture::PATH::explosion);
		ResourceManager::Get().LoadModel(Resource::Model::PATH::jeep);

		this->add_system<TestSystem>();
		this->Create<TestEntity>();
	}
	void Finalize(void) override {}
};