#include "scene.h"
#include "scene.h"
#include "scene.h"
#include "scene.h"

void Scene::Pause(void)
{
	for (auto && system : this->systems_)
		if (system.second)
		{
			system.second->Pause();
		}
		else
		{
			this->systems_.erase(system.first);
		}

	for (unsigned int n = 0; n < this->entities_.size(); ++n)
		if (this->entities_[n])
		{
			this->entities_[n]->Pause();
		}
		else
		{
			this->entities_.erase(this->entities_.begin() + n);
		}
}

void Scene::Update(void)
{
	for (auto && system : this->systems_)
		if (system.second)
		{
			system.second->Update();
		}
		else
		{
			this->systems_.erase(system.first);
		}

	for (unsigned int n = 0; n < this->entities_.size(); ++n)
		if (this->entities_[n])
		{
			this->entities_[n]->Update();
		}
		else
		{
			this->entities_.erase(this->entities_.begin() + n);
		}
}

void Scene::Always(void)
{
	for (auto && system : this->systems_)
		if (system.second)
		{
			system.second->Always();
		}
		else
		{
			this->systems_.erase(system.first);
		}

	for (unsigned int n = 0; n < this->entities_.size(); ++n)
		if (this->entities_[n])
		{
			this->entities_[n]->Always();
		}
		else
		{
			this->entities_.erase(this->entities_.begin() + n);
		}
}