#include "App.h"

#include "Input.h"
#include "Time.h"

#include "Player.h"
#include "GridMap.h"


bool App::m_running = true;

RaycastRenderer* App::m_renderer;

int App::Execute()
{
	Setup();

	Loop();

	Cleanup();

	return 0;
}

void App::Setup()
{
	m_renderer = new RaycastRenderer();

	Input::Initialize();
}

void App::Loop()
{
	while (m_running) 
	{
		FrameStart();
		FrameInput();
		FrameEvents();
		FrameRender();
		FrameEnd();
	}
}

void App::Cleanup()
{
	Input::Cleanup();

	if (m_renderer)
		delete m_renderer;
}

void App::FrameStart()
{
	Input::Update();
	Time::Update();
}

void App::FrameInput()
{
	m_running = Input::EventHandler();


	if (Input::GetKeyDown(SDL_SCANCODE_F11) && m_renderer)
	{
		m_renderer->ToggleFullscreen();
	}

	m_renderer->Inputs();
}

void App::FrameEvents()
{
	if (m_renderer)
		m_renderer->Update();
}

void App::FrameRender()
{
	if (!m_renderer) return;

	m_renderer->Render();
}

void App::FrameEnd()
{
}
