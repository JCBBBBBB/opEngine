#pragma once
#include "..\\opEngine_SOURCE\\opScript.h"


namespace op
{
	class PlayerScript : public Script
	{
	public:
		enum class State
		{
			SitDown,
			Walk,
			Sleep,
			Attack,
		};

		PlayerScript();
		~PlayerScript();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;

	private:
		void SitDown();
		void Move();

	private:
		State m_state;
		class Animator* m_animator;
	};
}
