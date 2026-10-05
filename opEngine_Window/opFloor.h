#pragma once
#include "..\\opEngine_SOURCE\\opGameObject.h"

namespace op
{
	class Floor : public GameObject
	{
	public:
		Floor();
		~Floor() override;

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;
	};
}