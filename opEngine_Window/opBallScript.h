#pragma once
#include "../opEngine_SOURCE/opScript.h"
#include "../opEngine_SOURCE/opGameObject.h"

namespace op
{
	class BallScript : public Script
	{
	public:
		BallScript();
		~BallScript();

		void Update() override;
		void Render(HDC hdc) override;

		void SetPaddle(GameObject* paddle) { m_paddle = paddle; }

		void Reset();

	private:
		void FollowPaddle();


	private:
		GameObject* m_paddle; // 공이 따라 갈 막대
		math::Vector2 m_velocity; // 초당 이동 거리
		float m_radius; // 반지름
		bool m_isLaunched; // 발사했는지
	};
}
