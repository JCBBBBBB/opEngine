#include "opApplication.h"
#include "opInput.h"
#include "opTime.h"

namespace op
{
	Application::Application() : m_hwnd(nullptr), m_hdc(nullptr), m_width(0), m_height(0), m_backHdc(NULL), m_backBitmap(NULL)
	{

	}

	Application::~Application()
	{

	}

	void Application::Initialize(HWND hwnd, UINT width, UINT height)
	{
		m_hwnd = hwnd;
		m_hdc = GetDC(hwnd);

		RECT rect = { 0, 0, width, height };
		AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);

		m_width = rect.right - rect.left;
		m_height = rect.bottom - rect.top;

		SetWindowPos(m_hwnd, nullptr, 0, 0, m_width, m_height, 0);
		ShowWindow(m_hwnd, true);

		// 윈도우 해상도에 맞는 백버퍼 생성
		m_backBitmap = CreateCompatibleBitmap(m_hdc, width, height);

		// 백버퍼를 가리킬 DC 생성
		m_backHdc = CreateCompatibleDC(m_hdc);

		// 도화지를 m_backBitmap으로 바꾼다
		HBITMAP oldBitmap = (HBITMAP)SelectObject(m_backHdc, m_backBitmap);
		DeleteObject(oldBitmap); // 이전 비트맵 삭제


		m_player.SetPosition(0, 0);

		Input::Initialize();
		Time::Initialize();
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
		Time::Update();
		
		m_player.Update();
	}

	void Application::LateUpdate()
	{

	}

	// 백버퍼에서 그림 그리고, 프론트버퍼에 복사해서 보여주는 거 반복
	void Application::Render()
	{
		Rectangle(m_backHdc, 0, 0, 800, 600); // 백버퍼에 배경 그리기

		Time::Render(m_backHdc); // 백버퍼에 글자 그리기
		m_player.Render(m_backHdc); // 실제 창에 플레이어 그리기

		// 백버퍼에 있는걸 프론트버퍼에 복사
		BitBlt(m_hdc, 0, 0, m_width, m_height, m_backHdc, 0, 0, SRCCOPY);
	}
}