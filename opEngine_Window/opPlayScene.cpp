
#include "opResultScene.h"
#include "../opEngine_SOURCE/opLayer.h"
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
	PlayScene::PlayScene() : m_paddle(nullptr), m_ball(nullptr), m_score(0), m_lives(3), m_stage(1), m_remainingBricks(0), m_isClear(false), m_resultScene(nullptr)
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
		paddleCollider->SetOffset(Vector2::Zero); // 패들의 위치를 그대로 충돌 영역의 왼쪽 위로 사용

		// 공
		m_ball = object::Instantiate<GameObject>(enums::LayerType::Player);
		m_ball->SetName(L"Ball");
		BallScript* ballScript = m_ball->AddComponent<BallScript>();
		ballScript->SetPaddle(m_paddle); // 공에게 따라갈 막대를 알려준다
		ballScript->SetPlayScene(this); // 공에게 현재 PlayScene의 주소를 알려준다 // 


		// 공 충돌 영역
		float radius = ballScript->GetRadius(); // 반지름
		float diameter = radius * 2.0f; // 지름

		BoxCollider2D* ballCollider = m_ball->AddComponent<BoxCollider2D>();
		ballCollider->SetSize(Vector2(diameter / 100.0f, diameter / 100.0f));
		ballCollider->SetOffset(Vector2(-radius, -radius));

		ballScript->Reset(); // 공을 발사 대기 상태로 만들고 패들 위에 놓는다
		


		Scene::Initialize();
		
	}

	void PlayScene::Update()
	{
		if (IsFinished()) // 게임 오버 또는 전체 클리어라면
		{
			if (m_resultScene != nullptr) // 연결된 결과 씬이 있다면
			{
				// 결과 화면에 최종값을 복사한다
				m_resultScene->SetResult(m_score, m_stage, m_isClear);
				SceneManager::LoadScene(L"ResultScene");
			}

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

			if (m_stage < MaxStage) // 마지막 스테이지 이전이면
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

		// 시작화면 또는 결과화면에서 들어오면 새 게임을 준비한다
		// Initialize()를 다시 호출하면 카메라/패들/공까지 중복 생성되므로, 재시작은 Restart()로 처리
		Restart();

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

	void PlayScene::Restart()
	{
		if (m_paddle == nullptr || m_ball == nullptr)
		{
			return;
		}

		PaddleScript* paddleScript = m_paddle->GetComponent<PaddleScript>();
		Transform* paddleTransform = m_paddle->GetComponent<Transform>();

		BallScript* ballScript = m_ball->GetComponent<BallScript>();
		Layer* brickLayer = GetLayer(enums::LayerType::Brick);

		if (paddleScript == nullptr || paddleTransform == nullptr || ballScript == nullptr || brickLayer == nullptr)
		{
			return;
		}

		// 이전 게임에서 남은 벽돌들을 삭제 예약한다
		// Break()를 호출하지 않으므로 점수는 올라가지 않는다
		for (GameObject* brick : brickLayer->GetGameObjects())
		{
			if (brick != nullptr)
			{
				object::Destroy(brick);
			}
		}

		// 반복문이 끝난 뒤 삭제 예약된 벽돌을 실제로 제거한다
		// 이 함수는 씬에 들어갈 때 실행하므로
		// 공의 충돌 처리 중에 벽돌을 삭제하는 상황이 아니다
		brickLayer->Destroy();

		m_score = 0;
		m_lives = 3;
		m_stage = 1;
		m_remainingBricks = 0;
		m_isClear = false;

		// 패들을 화면 아래 중앙으로 돌려놓는다.
		float width = static_cast<float>(application.GetWidth());
		float height = static_cast<float>(application.GetHeight());

		Vector2 paddleSize = paddleScript->GetSize();

		paddleTransform->SetPosition(Vector2((width - paddleSize.x) / 2.0f, height - 60.0f));

		ballScript->Reset(); // 공을 이동한 패들 위로 돌려놓는다
		CreateBricks(); // 1스테이지 벽돌 24개를 만든다
	}
}