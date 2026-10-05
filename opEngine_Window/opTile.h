#pragma once
#include "..\\opEngine_SOURCE\\opGameObject.h"

namespace op
{
	// 타일 하나를 나타내는 게임 오브젝트
	class Tile : public GameObject
	{
	public:
		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;
		
		// 픽셀 좌표가 아니라 격자의 열과 행을 받는다.
		void SetPosition(int x, int y);
	};
}