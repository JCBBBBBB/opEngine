#pragma once

#include "opGameObject.h"

namespace op
{
	class Application
	{
	public:
		Application();
		~Application();

		void Initialize(HWND hwnd, UINT width, UINT height);
		void Run();

		void Update();
		void LateUpdate();
		void Render();
		void Release();
		void Destroy();

		HDC GetHDC() { return m_hdc; }
		UINT GetWidth() { return m_width; }
		UINT GetHeight() { return m_height; }
		HWND GetHwnd() { return m_hwnd; }

	private:
		void ClearRenderTarget();
		void CopyRenderTarget(HDC source, HDC dest);
		void AdjustWindow(HWND hwnd, UINT width, UINT height);
		void CreateBuffer(UINT Width, UINT height);

	private:
		HWND m_hwnd; // 사용할 창의 핸들
		HDC m_hdc; // 그 창에 그리기 위한 DC 핸들

		HDC m_backHdc; // 메모리에 그리는데 사용할 DC
		HBITMAP m_backBitmap; // 그 그림을 저장할 비트맵

		UINT m_width;
		UINT m_height;

	};
}