#pragma once
#include "CommonInclude.h"

namespace op
{
	class GameObject
	{
	public:
		GameObject();
		~GameObject();

		void Update();
		void LateUpdate();
		void Render(HDC hdc);

		void SetPosition(float x, float y)
		{
			m_x = x;
			m_y = y;
		}

		float GetPositionX() { return m_x; }
		float GetPositionY() { return m_y; }

	private:
		float m_x;
		float m_y;
	};
}
