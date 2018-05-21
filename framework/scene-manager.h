#pragma once

#include <memory>

#include "scene.h"

class SceneManager
{
public:
	SceneManager(void) noexcept(false);
	SceneManager(SceneManager && move_from) noexcept;
	SceneManager & operator= (SceneManager && move_from) noexcept;

	SceneManager(SceneManager const &) = delete;
	SceneManager & operator=(SceneManager const &) = delete;

	virtual ~SceneManager(void);

private:
	std::shared_ptr<Scene> current_scene_;
	std::shared_ptr<Scene> next_scene_;

public:
	bool Run(void);
	const std::weak_ptr<Scene> current_scene(void) const;
	const std::weak_ptr<Scene> next_scene(void) const;

	template<class _Scene, class ... Args> void Initalize(const Args &... args)
	{
		this->next_scene_ = std::make_shared<_Scene>(args ...);
	}
	template<class _Scene, class ... Args> void SetScene(const Args &... args)
	{
		this->next_scene_ = std::make_shared<_Scene>(args ...);
	}

private:
	bool pause_ = false;
	bool running_ = true;

public:
	const bool & pause(void) const;
	const bool & running(void) const;
	
	void Pause(void);
	void Resume(void);
	void Shutdown(void);

public:
	// singleton
	static SceneManager & __cdecl Get(void);

private:
	class Impl;

	std::unique_ptr<Impl> impl_;
};