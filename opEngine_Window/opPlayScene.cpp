
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
	PlayScene::PlayScene() : m_paddle(nullptr), m_ball(nullptr), m_score(0), m_lives(3)
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
			}
		}


		Scene::Initialize();
		
	}

	void PlayScene::Update()
	{
		// 게임이 끝나면 객체들이 Update를 실행하지 않는다
		// 패들 조작과 공 이동이 멈춘다
		if (IsGameOver())
		{
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
		// 먼저 패들·공·벽돌을 그린다.
		Scene::Render(hdc);

		// 숫자를 문자열로 바꾼다.
		// 예: m_score가 30이면 "Score : 30"
		std::wstring text =
			L"Score : " + std::to_wstring(m_score)
			+ L"   Lives : " + std::to_wstring(m_lives);
		
		// 글자 배경을 따로 칠하지 않고 흰색 글자로 표시한다.
		int oldBackgroundMode = SetBkMode(hdc, TRANSPARENT);
		COLORREF oldTextColor = SetTextColor(hdc, RGB(255, 255, 255));

		// 화면 왼쪽 위 (20, 40)에 점수를 그린다.
		// UI이므로 카메라 좌표 변환을 하지 않는다.
		TextOutW(
			hdc,
			20,
			40,
			text.c_str(),
			static_cast<int>(text.length())
		);

		if (IsGameOver())
		{
			// 화면 전체 범위를 지정한다
			RECT area = {};
			area.right = static_cast<LONG>(application.GetWidth());
			area.bottom = static_cast<LONG>(application.GetHeight());
		
			// 화면의 가로·세로 중앙에 한 줄로 표시한다.
			DrawTextW(
				hdc,
				L"GAME OVER",
				-1,
				&area,
				DT_CENTER | DT_VCENTER | DT_SINGLELINE
			);
		}


		// 다른 그리기에 영향을 주지 않도록 설정을 복원한다.
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
		if (IsGameOver())
		{
			return;
		}

		m_lives--;
	}
}