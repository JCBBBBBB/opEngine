#pragma once
#include "..\\opEngine_SOURCE\\opScene.h"

namespace op
{
	// 타일맵을 만드는 작업용 씬
	class ToolScene : public Scene
	{
	public:
		ToolScene();
		~ToolScene() override;

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;

		void OnEnter() override;
		void OnExit() override;
	};
}

LRESULT CALLBACK WndTileProc(HWND, UINT, WPARAM, LPARAM);
