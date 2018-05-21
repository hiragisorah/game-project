#pragma once

#include <memory>
#include <unordered_map>
#include <vector>

#include "renderer.h"

class Graphics
{
public:
	Graphics(void) noexcept(false);
	Graphics(Graphics && move_from) noexcept;
	Graphics & operator= (Graphics && move_from) noexcept;

	Graphics(Graphics const &) = delete;
	Graphics & operator=(Graphics const &) = delete;

	virtual ~Graphics(void);

public:
	virtual void Initialize(void) = 0;
	virtual void Finalize(void) = 0;

	virtual void Clear(void) = 0;
	virtual void Present(void) = 0;

private:
	std::unordered_map<DRAW_MODE, std::vector<std::weak_ptr<Renderer>>> renderer_list_;

public:
	void AddRenderer(const std::shared_ptr<Renderer> & renderer);
	void Rendering(void);

public:
	virtual void Rendering(const std::weak_ptr<Renderer> & renderer) = 0;


public:
	// singleton
	static Graphics & __cdecl Get(void);

private:
	class Impl;

	std::unique_ptr<Impl> impl_;
};