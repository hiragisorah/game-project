#pragma once

enum DRAW_MODE
{
	DRAW_MODE_BACK_BUFFER_2D,
	DRAW_MODE_BACK_BUFFER_3D,
	DRAW_MODE_DEFFERED_2D,
	DRAW_MODE_DEFFERED_3D
};

struct Renderer
{
	Renderer(void)
		: draw_mode_(DRAW_MODE_BACK_BUFFER_2D)
	{}

	DRAW_MODE draw_mode_;
};