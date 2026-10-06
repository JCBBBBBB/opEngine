#pragma once
#include "../opEngine_SOURCE/opScript.h"

namespace op
{
	class PaddleScript : public Script
	{
	public:
		PaddleScript();
		~PaddleScript();

		void Update() override;
		void Render(HDC hdc) override;

		math::Vector2 GetSize() const { return m_size; }

	private:
		float m_speed; // 초당 이동 거리
		math::Vector2 m_size; // 막대의 너비와 높이
	};
}
