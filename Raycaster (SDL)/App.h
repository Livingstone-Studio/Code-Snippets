#pragma once

#include "RaycastRenderer.h"

#include <iostream>

using namespace std;

class App
{
public:
	static int Execute();

private:
	static void Setup();
	static void Loop();
	static void Cleanup();

	static void FrameStart();
	static void FrameInput();
	static void FrameEvents();
	static void FrameRender();
	static void FrameEnd();

	static RaycastRenderer* m_renderer;
	static bool m_running;
};

