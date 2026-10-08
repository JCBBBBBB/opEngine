#pragma once
#include "../opEngine_SOURCE/opScript.h"
#include "../opEngine_Window/opPlayScene.h"

namespace op
{
	class BrickScript : public Script
	{
	public:
		BrickScript();
		~BrickScript();

		void Render(HDC hdc) override;
		math::Vector2 GetSize() const { return m_size; }

		void SetPlayScene(PlayScene* scene) { m_playScene = scene; } // 이 벽돌이 점수를 전달한 씬을 저장

		void Break(); // 벽돌을 부수고 삭제를 예약한다

	private:
		math::Vector2 m_size;
		bool m_isBroken;
		PlayScene* m_playScene; // 점수를 관리하는 씬의 주소
	};
}
