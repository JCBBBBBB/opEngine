#pragma once
#include "..\\opEngine_SOURCE\\opGameObject.h"

namespace op
{
	class Cat : public GameObject
	{
		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;

	};
}
