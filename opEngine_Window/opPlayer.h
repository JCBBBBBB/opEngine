#pragma once
#include "..\\opEngine_SOURCE\\opGameObject.h"


namespace op
{
	class Player : public GameObject
	{
	public:
		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;

	private:
		int m_hp = 0;
	};
}
