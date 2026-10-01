#pragma once

#include "opGameObject.h"

namespace op
{
	class Application
	{
	public:
		Application();
		~Application();

		void Initialize(HWND hwnd);
		void Run();

		void Update();
		void LateUpdate();
		void Render();

	private:
		HWND m_hwnd; // 사용할 창의 핸들
		HDC m_hdc; // 그 창에 그리기 위한 DC 핸들

		GameObject m_player;
	};
}