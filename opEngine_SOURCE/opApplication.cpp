#include "opApplication.h"
#include "opGameObject.h"

namespace op
{
	Application::Application() : m_hwnd(nullptr), m_hdc(nullptr)
	{

	}

	Application::~Application()
	{

	}

	void Application::Initialize(HWND hwnd)
	{
		m_hwnd = hwnd;
		m_hdc = GetDC(hwnd);

		m_player.SetPosition(0, 0);
	}

	void Application::Run()
	{
		Update();
		LateUpdate();
		Render();
	}


	void Application::Update()
	{
		m_player.Update();
	}

	void Application::LateUpdate()
	{

	}


	void Application::Render()
	{
		m_player.Render(m_hdc);
	}
}