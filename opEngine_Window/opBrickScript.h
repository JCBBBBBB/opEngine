#pragma once
#include "../opEngine_SOURCE/opScript.h"

namespace op
{
	class BrickScript : public Script
	{
	public:
		BrickScript();
		~BrickScript();

		void Render(HDC hdc) override;
		math::Vector2 GetSize() const { return m_size; }

		void Break(); // 벽돌을 부수고 삭제를 예약한다

	private:
		math::Vector2 m_size;
		bool m_isBroken;
	};
}
