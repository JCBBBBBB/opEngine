#pragma once
#include "..\\opEngine_SOURCE\\opScript.h"
#include "..\\opEngine_SOURCE\\opTexture.h"

namespace op
{
	class PlayerScript : public Script
	{
	public:
		enum class State
		{
			Idle,
			Walk,
			Sleep,
			GiveWater,
			Attack,
		};

		PlayerScript();
		~PlayerScript();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;

		void AttackEffect();
        void OnCollisionEnter(Collider* other) override;
        void OnCollisionStay(Collider* other) override;
        void OnCollisionExit(Collider* other) override;

		// 충돌 검사에 사용할 이미지를 연결한다.
		void SetPixelMapTexture(graphics::Texture* texture)
		{
			m_pixelMap = texture;
		}
	private:
		void Idle();
		void Move();
		void GiveWater();

	private:
		State m_state;
		class Animator* m_animator;

		graphics::Texture* m_pixelMap; // 발밑의 색을 검사할 이미지
	};
}
