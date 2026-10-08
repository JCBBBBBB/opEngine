
#include "opAudioClip.h"
#include "opAudioListener.h"
#include "opAudioSource.h"
#include "opUIManager.h"
#include "opBrickScript.h"
#include "..\\opEngine_SOURCE\\opRigidbody.h"
#include "opTile.h"
#include "..\\opEngine_SOURCE\\opTilemapRenderer.h"
#include <cstdio>
#include <string>
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
	PlayScene::PlayScene() : m_paddle(nullptr), m_ball(nullptr), m_score(0), m_lives(3), m_stage(1), m_remainingBricks(0), m_isClear(false)
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
		ballScript->SetPlayScene(this);


		// 공 충돌 영역
		float radius = ballScript->GetRadius();
		float diameter = radius * 2.0f;

		BoxCollider2D* ballCollider = m_ball->AddComponent<BoxCollider2D>();
		ballCollider->SetSize(Vector2(diameter / 100.0f, diameter / 100.0f));
		ballCollider->SetOffset(Vector2(-radius, -radius));

		ballScript->Reset();

		CreateBricks();
		


		Scene::Initialize();
		
	}

	void PlayScene::Update()
	{
		// 게임이 끝나면 객체들이 Update를 실행하지 않는다
		// 패들 조작과 공 이동이 멈춘다
		if (IsFinished())
		{
			return;
		}

		// 이전 프레임에서 마지막 벽돌까지 부서졌다면
		if (m_remainingBricks == 0)
		{
			// 공을 패들 위로 돌려놓고 발사 대기 상태로 만든다
			if (m_ball != nullptr)
			{
				BallScript* ballScript = m_ball->GetComponent<BallScript>();

				if (ballScript != nullptr)
				{
					ballScript->Reset();
				}
			}

			if (m_stage < MaxStage)
			{
				m_stage++;

				CreateBricks();
			}
			else
			{
				m_isClear = true;
			}

			return;
		}

		Scene::Update();
	}

	void PlayScene::LateUpdate()
	{
		Scene::LateUpdate();
	}

	void PlayScene::Render(HDC hdc)
	{
		Scene::Render(hdc);

		std::wstring text =
			L"Stage : " + std::to_wstring(m_stage)
			+ L"   Score : " + std::to_wstring(m_score)
			+ L"   Lives : " + std::to_wstring(m_lives);

		int oldBackgroundMode = SetBkMode(hdc, TRANSPARENT);
		COLORREF oldTextColor = SetTextColor(hdc, RGB(255, 255, 255));

		TextOutW(
			hdc,
			20,
			40,
			text.c_str(),
			static_cast<int>(text.length())
		);

		if (IsFinished())
		{
			RECT area = {};
			area.right = static_cast<LONG>(application.GetWidth());
			area.bottom = static_cast<LONG>(application.GetHeight());

			// 모든 스테이지를 끝냈으면 ALL CLEAR,
			// 목숨을 모두 잃었으면 GAME OVER를 표시한다.
			const wchar_t* message =
				m_isClear ? L"ALL CLEAR" : L"GAME OVER";

			DrawTextW(
				hdc,
				message,
				-1,
				&area,
				DT_CENTER | DT_VCENTER | DT_SINGLELINE
			);
		}

		SetTextColor(hdc, oldTextColor);
		SetBkMode(hdc, oldBackgroundMode);
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

	void PlayScene::AddScore(int score)
	{
		m_score += score;
	}

	void PlayScene::LoseLife()
	{
		if (IsFinished())
		{
			return;
		}

		m_lives--;
	}

	void PlayScene::OnBrickBroken()
	{
		// 게임 종료 후거나 이미 벽돌이 없는 경우에는 처리하지 않는다
		if (IsFinished() || m_remainingBricks <= 0)
		{
			return;
		}

		AddScore(10);

		m_remainingBricks--;
	}

	void PlayScene::CreateBricks()
	{
		float width = static_cast<float>(application.GetWidth());

		const int rowCount = m_stage + 3;
		const int columnCount = 6;

		const float gapX = 10.0f;
		const float gapY = 10.0f;
		const float startY = 100.0f;

		m_remainingBricks = 0;

		// 첫번째 줄부터 네번째 줄까지 반복
		for (int row = 0; row < rowCount; row++)
		{
			for (int column = 0; column < columnCount; column++)
			{
				// 벽돌 객체 생성
				GameObject* brick = object::Instantiate<GameObject>(enums::LayerType::Brick);
				brick->SetName(L"Brick");

				BrickScript* brickScript = brick->AddComponent<BrickScript>();
				brickScript->SetPlayScene(this); // 벽돌에게 점수를 관리하는 현재 PlayScene을 알려준다

				Vector2 brickSize = brickScript->GetSize();

				float totalWidth = columnCount * brickSize.x + (columnCount - 1) * gapX;
				float startX = (width - totalWidth) / 2.0f;
				float x = startX + column * (brickSize.x + gapX);
				float y = startY + row * (brickSize.y + gapY);

				Transform* brickTransform = brick->GetComponent<Transform>();
				brickTransform->SetPosition(Vector2(x, y));

				BoxCollider2D* brickCollider = brick->AddComponent<BoxCollider2D>();
				brickCollider->SetSize(brickSize / 100.0f);
				brickCollider->SetOffset(Vector2::Zero);

				m_remainingBricks++;
			}
		}
	}
}