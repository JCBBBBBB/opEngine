#include "opUIManager.h"
#include "opApplication.h"
#include "opInput.h"
#include "opTime.h"
#include "opSceneManager.h"
#include "opResources.h"
#include "opCollisionManager.h"

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
		AdjustWindow(hwnd, width, height);
		CreateBuffer(width, height);

		Input::Initialize();
		Time::Initialize();

		Fmod::Initialize();
		CollisionManager::Initialize();
		UIManager::Initialize();
		SceneManager::Initialize();
	}

	void Application::Run()
	{
		Update();
		LateUpdate();
		Render();

		Destroy();
	}


	void Application::Update()
	{
		Input::Update();
		Time::Update();

		UIManager::Update();
		SceneManager::Update();
	}

	void Application::LateUpdate()
	{
		UIManager::LateUpdate();
		SceneManager::LateUpdate();

		CollisionManager::Update();
		CollisionManager::LateUpdate();

		// 위치 전달과 충돌에 따른 재생 요청 후 FMOD 갱신
		Fmod::Update();
	}

	void Application::Render()
	{
		ClearRenderTarget();

		Time::Render(m_backHdc);
		CollisionManager::Render(m_backHdc);

		UIManager::Render(m_backHdc);
		SceneManager::Render(m_backHdc);

		CopyRenderTarget(m_backHdc, m_hdc);
	}

	void Application::Release()
	{
		SceneManager::Release();
		UIManager::Release();
		Resources::Release();
		Fmod::Release();

        if (m_backHdc)
        {
            SelectObject(m_backHdc, m_oldBackBitmap);
            DeleteObject(m_backBitmap);
            DeleteDC(m_backHdc);
            m_backHdc = nullptr;
            m_backBitmap = nullptr;
            m_oldBackBitmap = nullptr;
        }
        if (m_hdc) { ReleaseDC(m_hwnd, m_hdc); m_hdc = nullptr; }
	}

	void Application::Destroy()
	{
		SceneManager::Destroy();
	}



	void Application::ClearRenderTarget()
	{
        HBRUSH brush = CreateSolidBrush(RGB(128, 128, 128));
        RECT area = { 0, 0, static_cast<LONG>(m_width), static_cast<LONG>(m_height) };
        FillRect(m_backHdc, &area, brush);
        DeleteObject(brush);
	}

	void Application::CopyRenderTarget(HDC source, HDC dest)
	{
		BitBlt(dest, 0, 0, m_width, m_height, source, 0, 0, SRCCOPY); 	// 백버퍼에 있는걸 프론트버퍼에 복사

	}


	void Application::AdjustWindow(HWND hwnd, UINT width, UINT height)
	{
        m_hwnd = hwnd;
        m_hdc = GetDC(hwnd);
        m_width = width;
        m_height = height;
        RECT rect = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
        AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
        SetWindowPos(hwnd, nullptr, 0, 0, rect.right - rect.left, rect.bottom - rect.top, SWP_NOZORDER);
	}

	void Application::CreateBuffer(UINT Width, UINT height)
	{
        m_backBitmap = CreateCompatibleBitmap(m_hdc, static_cast<int>(m_width), static_cast<int>(m_height));
        m_backHdc = CreateCompatibleDC(m_hdc);
        m_oldBackBitmap = static_cast<HBITMAP>(SelectObject(m_backHdc, m_backBitmap));
	}
}