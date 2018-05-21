#pragma once

#include <memory>

#include "graphics.h"
#include "scene-manager.h"

class Engine
{
public:
	Engine(void) noexcept(false);
	Engine(Engine && move_from) noexcept;
	Engine & operator= (Engine && move_from) noexcept;

	Engine(Engine const &) = delete;
	Engine & operator=(Engine const &) = delete;

	virtual ~Engine(void);

private:
	std::unique_ptr<SceneManager> scene_manager_;
	std::unique_ptr<Graphics> graphics_;

public:
	// singleton
	static Engine & __cdecl Get(void);

private:
	class Impl;

	std::unique_ptr<Impl> impl_;
};