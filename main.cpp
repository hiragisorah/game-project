#include <iostream>

#include "framework\scene-manager.h"
#include "simple-window.h"
#include "directx11.h"
#include "test-scene.h"

#pragma comment(lib, "framework.lib")
#pragma comment(lib, "graphics.lib")


#ifdef _DEBUG
#define DEBUG
int main(void)
{
	// メモリーリーク検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#else
int __stdcall WinMain(HINSTANCE, HINSTANCE, char*, int)
#endif

	SceneManager scene_manager;
	SimpleWindow window("test", 1280U, 720U);
	DirectX11 graphics(&window);

	scene_manager.Initalize<TestScene>();
	window.Initialize();
	graphics.Initialize();

	while (scene_manager.Run() && window.Run() && graphics.Run());

	return 0;
}