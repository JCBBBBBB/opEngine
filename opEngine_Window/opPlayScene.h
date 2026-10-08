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
	private:
		GameObject* m_paddle;
		GameObject* m_ball;

		int m_score;
		int m_lives; // 남은 목숨
	};
}
