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
	};
}