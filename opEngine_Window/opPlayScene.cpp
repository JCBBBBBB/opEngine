
#include "opAudioClip.h"
#include "opAudioListener.h"
#include "opAudioSource.h"
#include "opUIManager.h"
#include "opBrickScript.h"
#include "..\\opEngine_SOURCE\\opRigidbody.h"
#include "opTile.h"
#include "..\\opEngine_SOURCE\\opTilemapRenderer.h"
#include <cstdio>

#include "opCat.h"
#include "opCatScript.h"

#include "opPlayerScript.h"
#include "opCamera.h"
#include "opRenderer.h"

#include "opFloor.h"
#include "opFloorScript.h"
#include "..\\opEngine_SOURCE\\opBoxCollider2D.h"

#include "opTexture.h"
#include "opResources.h"

#include "opPlayScene.h"
#include "opGameObject.h"
#include "opPlayer.h"
#include "opTransform.h"
#include "opSpriteRenderer.h"
#include "opInput.h"
#include "opObject.h"
#include "opTitleScene.h"
#include "opSceneManager.h"
#include "opAnimator.h"
#include "opCircleCollider2D.h"
#include "opCollisionManager.h"
#include "opPaddleScript.h"
#include "../opEngine_SOURCE/opApplication.h"

extern op::Application application;

namespace op
{
	PlayScene::PlayScene() : m_paddle(nullptr), m_ball(nullptr)
	{

	}

	PlayScene::~PlayScene()
	{

	}

	void PlayScene::Initialize()
	{
		float width = static_cast<float>(application.GetWidth());
		float height = static_cast<float>(application.GetHeight());

		// 카메라
		GameObject* camera = object::Instantiate<GameObject>(enums::LayerType::None, Vector2(width / 2.0f, height / 2.0f));
		Camera* cameraComponent = camera->AddComponent<Camera>();
		renderer::mainCamera = cameraComponent;

		// 패들
		m_paddle = object::Instantiate<GameObject>(enums::LayerType::Player);
		m_paddle->SetName(L"Paddle");
		PaddleScript* paddleScript = m_paddle->AddComponent<PaddleScript>();
		Vector2 paddleSize = paddleScript->GetSize();

		Transform* paddleTransform = m_paddle->GetComponent<Transform>();
		paddleTransform->SetPosition(Vector2((width - paddleSize.x) / 2.0f, height - 60.0f));

		BoxCollider2D* paddleCollider = m_paddle->AddComponent<BoxCollider2D>();
		paddleCollider->SetSize(paddleSize / 100.0f);
		paddleCollider->SetOffset(Vector2::Zero);

		// 공
		m_ball = object::Instantiate<GameObject>(enums::LayerType::Player);
		m_ball->SetName(L"Ball");
		BallScript* ballScript = m_ball->AddComponent<BallScript>();
		ballScript->SetPaddle(m_paddle); // 공에게 따라갈 막대를 알려준다

		// 공 충돌 영역
		float radius = ballScript->GetRadius();
		float diameter = radius * 2.0f;

		BoxCollider2D* ballCollider = m_ball->AddComponent<BoxCollider2D>();
		ballCollider->SetSize(Vector2(diameter / 100.0f, diameter / 100.0f));
		ballCollider->SetOffset(Vector2(-radius, -radius));

		ballScript->Reset();

		const int rowCount = 4;
		const int columnCount = 6;

		const float gapX = 10.0f;
		const float gapY = 10.0f;
		const float startY = 100.0f;

		// 첫번째 줄부터 네번째 줄까지 반복
		for (int row = 0; row < rowCount; row++)
		{
			for (int column = 0; column < columnCount; column++)
			{
				// 벽돌 객체 생성
				GameObject* brick = object::Instantiate<GameObject>(enums::LayerType::Brick);
				brick->SetName(L"Brick");

				BrickScript* brickScript = brick->AddComponent<BrickScript>();
				Vector2 brickSize = brickScript->GetSize();

				float totalWidth = columnCount * brickSize.x + (columnCount - 1) * gapX;
				float startX = (width - totalWidth) / 2.0f;
				float x = startX + column * (brickSize.x + gapX);
				float y = startY + column * (brickSize.y + gapY);

				Transform* brickTransform = brick->GetComponent<Transform>();
				brickTransform->SetPosition(Vector2(x, y));

				BoxCollider2D* brickCollider = brick->AddComponent<BoxCollider2D>();
				brickCollider->SetSize(brickSize / 100.0f);
				brickCollider->SetOffset(Vector2::Zero);
			}
		}


		Scene::Initialize();
		
	}

	void PlayScene::Update()
	{
		Scene::Update();
	}

	void PlayScene::LateUpdate()
	{
		Scene::LateUpdate();
	}

	void PlayScene::Render(HDC hdc)
	{
		Scene::Render(hdc);
	}

	void PlayScene::OnEnter()
	{
		Scene::OnEnter();
		CollisionManager::CollisionLayerCheck(enums::LayerType::Player, enums::LayerType::Player, true);
		CollisionManager::CollisionLayerCheck(enums::LayerType::Player, enums::LayerType::Brick, true);
	}

	void PlayScene::OnExit()
	{
		Scene::OnExit();
	}
}