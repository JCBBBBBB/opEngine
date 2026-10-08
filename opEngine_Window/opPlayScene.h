#pragma once
#include "..\opEngine_SOURCE\\opScene.h"
#include "..\opEngine_SOURCE\\opGameObject.h"
#include "../opEngine_SOURCE/opBoxCollider2D.h"
#include "../opEngine_SOURCE/opCollisionManager.h"
#include "opBallScript.h"

namespace op
{
	class PlayScene : public Scene
	{
	public:
		PlayScene();
		~PlayScene();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;

		void OnEnter() override;
		void OnExit() override;

		void AddScore(int score); // 벽돌이 부서졌을 때 호출해서 점수를 올린다
		void LoseLife(); // 공을 놓쳤을 때 목숨을 줄인다

		bool IsGameOver() const { return m_lives <= 0; } // 남은 목숨이 0이면 true, 남아 있으면 false
		void OnBrickBroken(); // 벽돌이 부서졌을 때 점수와 남은 벽돌 수를 갱신
		bool IsFinished() const { return IsGameOver() || m_isClear; } // 목숨을 모두 잃었거나, 모든 스테이지를 끝냈는지 확인한다
	
	private:
		void CreateBricks(); // 현재 스테이지의 벽돌을 생성한다

	private:
		GameObject* m_paddle;
		GameObject* m_ball;

		int m_score;
		int m_lives; // 남은 목숨

		int m_stage; // 현재 스테이지 번호
		int m_remainingBricks; // 아직 부서지지 않은 벽돌 수
		bool m_isClear; // 모든 스테이지를 끝냈는지
	
		static constexpr int MaxStage = 3;
	};
}
