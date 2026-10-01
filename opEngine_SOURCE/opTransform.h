#pragma once

#include "opEntity.h"
#include "opComponent.h"

namespace op
{
	struct Pos
	{
		int m_x;
		int m_y;
	};

	class Transform : public Component
	{
	public:
		Transform();
		~Transform();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;

		void SetPos(int x, int y) { m_x = x; m_y = y; }
		int GetPosX() { return m_x; }
		int GetPosY() { return m_y; }

	private:
		int m_x;
		int m_y;
	};
}
