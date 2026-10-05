#pragma once
#include "..\\opEngine_SOURCE\\opScript.h"
#include "..\\opEngine_SOURCE\\opTransform.h"

namespace op
{
	class CatScript : public Script
	{
	public:
		enum class State
		{
			SitDown,
			Walk,
			Sleep,
			LayDown,
			Attack,
		};

		enum class Direction
		{
			Left,
			Right,
			Down,
			Up,
			End,
		};

		CatScript();
		~CatScript();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;
		void SetPlayer(GameObject* player)
		{
			m_player = player;
		}

		Vector2 m_dest;
	private:
		void SitDown();
		void Move();
		void LayDown();

		void PlayWalkAnimationByDirection(Direction dir);
		void Translate(Transform* tr);

	private:
		State m_state; // 상태
		class Animator* m_animator; // 
		Direction m_direction; // 선택한 이동 방향
		float m_time; // 현재 행동에서 누적한 시간
		float m_deathTime;
		GameObject* m_player;
		float m_radian;
	};
}
