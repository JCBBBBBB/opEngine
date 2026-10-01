#include "opApplication.h"
#include "opInput.h"

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

		Input::Initialize();
	}

	void Application::Run()
	{
		Update();
		LateUpdate();
		Render();
	}


	void Application::Update()
	{
		Input::Update(); // 입력 먼저 갱신한 다음 플레이어 갱신해야 함
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