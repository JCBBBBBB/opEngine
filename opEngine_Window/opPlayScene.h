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

	private:
		GameObject* m_paddle;
		GameObject* m_ball;
	};
}
