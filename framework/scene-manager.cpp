#include "scene-manager.h"
#include "scene-manager.h"
#include "scene-manager.h"
#include "scene-manager.h"
#include "scene-manager.h"

class SceneManager::Impl
{
public:
	Impl(SceneManager * scene_manager)
		: owner_(scene_manager)
	{
		if (this->singleton_)
		{
			throw std::exception("scene_manager is singleton.");
		}

		Impl::singleton_ = this;
	}

	virtual ~Impl(void)
	{
		Impl::singleton_ = nullptr;
	}

	SceneManager * owner_;

	static Impl * singleton_;
};

SceneManager::Impl * SceneManager::Impl::singleton_ = nullptr;

SceneManager::SceneManager(void) noexcept(false)
	: impl_(std::make_unique<Impl>(this))
{

}

SceneManager::SceneManager(SceneManager && move_from) noexcept
	: impl_(std::move(move_from.impl_))
{
	this->impl_->owner_ = this;
}

SceneManager & SceneManager::operator=(SceneManager && move_from) noexcept
{
	this->impl_ = std::move(move_from.impl_);
	this->impl_->owner_ = this;
	return *this;
}

SceneManager::~SceneManager(void)
{

}

bool SceneManager::Run(void)
{
	if (this->current_scene_)
	{
		if (this->pause_)
		{
			this->current_scene_->Pause();
		}
		else
		{
			this->current_scene_->Update();
		}

		this->current_scene_->Always();
	}

	if (this->next_scene_)
	{
		if(this->current_scene_)
			this->current_scene_->Finalize();

		this->current_scene_.swap(this->next_scene_);
		this->next_scene_.reset();

		this->current_scene_->Initialize();
	}
	else
	{
		this->running_ = this->current_scene_ != nullptr;
	}

	return this->running_;
}

const std::weak_ptr<Scene> SceneManager::current_scene(void) const
{
	return this->current_scene_;
}

const std::weak_ptr<Scene> SceneManager::next_scene(void) const
{
	return this->next_scene_;
}

const bool & SceneManager::pause(void) const
{
	return this->pause_;
}

const bool & SceneManager::running(void) const
{
	return this->running_;
}

void SceneManager::Pause(void)
{
	this->pause_ = true;
}

void SceneManager::Resume(void)
{
	this->pause_ = false;
}

void SceneManager::Shutdown(void)
{
	this->running_ = false;
}

SceneManager & SceneManager::Get(void)
{
	if (!Impl::singleton_ || !Impl::singleton_->owner_)
		throw std::exception("scene_manager is a singleton.");

	return *Impl::singleton_->owner_;
}