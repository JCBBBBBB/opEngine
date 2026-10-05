#pragma once
#include "..\\opEngine_SOURCE\\opScene.h"
#include "opTile.h"

namespace op
{
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

		void Save();
		void Load();

	private:
		// 저장할 타일 목록. 타일의 실제 삭제는 Scene/Layer가 담당한다.
		std::vector<Tile*> m_tiles;
	};
}

LRESULT CALLBACK WndTileProc(HWND, UINT, WPARAM, LPARAM);