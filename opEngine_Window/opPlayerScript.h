#pragma once
#include "..\\opEngine_SOURCE\\opScript.h"


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

	private:
		void Idle();
		void Move();
		void GiveWater();

	private:
		State m_state;
		class Animator* m_animator;
	};
}
