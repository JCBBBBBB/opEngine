#pragma once
#include "../opEngine_SOURCE/opScript.h"
#include "../opEngine_SOURCE/opGameObject.h"
#include "../opEngine_Window/opPlayScene.h"

namespace op
{
	class PlayScene;

	class BallScript : public Script
	{
	public:
		BallScript();
		~BallScript();

		void Update() override;
		void Render(HDC hdc) override;

		void SetPaddle(GameObject* paddle) { m_paddle = paddle; }

		void Reset();

		void OnCollisionEnter(Collider* other) override;
		void OnCollisionStay(Collider* other) override;

		float GetRadius() const { return m_radius; }
		
		void SetPlayScene(PlayScene* scene) { m_playScene = scene; }
	private:
		void FollowPaddle();
		void ReflectBrick(Collider* other);

	private:
		GameObject* m_paddle; // 공이 따라 갈 막대
		math::Vector2 m_velocity; // 초당 이동 거리
		math::Vector2 m_previousPosition; // 이동하기 전 공 위치
		float m_radius; // 반지름
		bool m_isLaunched; // 발사했는지
		PlayScene* m_playScene; // 목숨을 관리하는 씬
	};
}
