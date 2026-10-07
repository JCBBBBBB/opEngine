
#include "opAudioClip.h"
#include "opAudioListener.h"
#include "opAudioSource.h"
#include "opUIManager.h"

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

		GameObject* camera = object::Instantiate<GameObject>(enums::LayerType::None, Vector2(width / 2.0f, height / 2.0f));
		Camera* cameraComponent = camera->AddComponent<Camera>();
		renderer::mainCamera = cameraComponent;

		m_paddle = object::Instantiate<GameObject>(enums::LayerType::Player);
		m_paddle->SetName(L"Paddle");
		PaddleScript* paddleScript = m_paddle->AddComponent<PaddleScript>();
		Vector2 paddleSize = paddleScript->GetSize();

		Transform* paddleTransform = m_paddle->GetComponent<Transform>();
		paddleTransform->SetPosition(Vector2((width - paddleSize.x) / 2.0f, height - 60.0f));

		m_ball = object::Instantiate<GameObject>(enums::LayerType::Player);
		m_ball->SetName(L"Ball");
		BallScript* ballScript = m_ball->AddComponent<BallScript>();
		ballScript->SetPaddle(m_paddle); // 공에게 따라갈 막대를 알려준다
		ballScript->Reset();

		Scene::Initialize();
		//// main camera
		//GameObject* camera = object::Instantiate<GameObject>(
		//	enums::LayerType::Particle,
		//	Vector2(336.0f, 423.0f));		Camera* cameraComponent = camera->AddComponent<Camera>();
		//renderer::mainCamera = cameraComponent;

		//// 플레이어
		//m_player = object::Instantiate<Player>(enums::LayerType::Player); // 플레이어 생성, 생성자에서 Transform이 자동으로 붙음
  //      object::DontDestroyOnLoad(m_player);
		//// 플레이어를 소리를 듣는 쪽으로 등록
		//m_player->AddComponent<AudioListener>();

		//BoxCollider2D* collider = m_player->AddComponent<BoxCollider2D>();
		//collider->SetOffset(Vector2(-50.0f, -50.0f));
  //      collider->SetOffset(Vector2(-50.0f, -50.0f));

  //      PlayerScript* playerScript = m_player->AddComponent<PlayerScript>(); // 방향키 이동 기능 붙인다

		//graphics::Texture* playerTexture = Resources::Find<graphics::Texture>(L"Player"); // LoadResources()에서 Cat으로 등록한 bmp파일 찾음, Resources 목록에서 chicken 찾기
		//Animator* playerAnimator = m_player->AddComponent<Animator>(); // 애니메이션을 관리하고 그리는 컴포넌트 추가
		//playerAnimator->CreateAnimation(L"Idle", playerTexture, Vector2(2000.0f, 250.0f), Vector2(250.0f, 250.0f), Vector2::Zero, 1, 0.1f);
		//playerAnimator->CreateAnimation(L"FrontGiveWater", playerTexture, Vector2(0.0f, 2000.0f), Vector2(250.0f, 250.0f), Vector2::Zero, 12, 0.1f);

		//playerAnimator->PlayAnimation(L"Idle", false); // 처음에는 대기 애니메이션
		//playerAnimator->GetCompleteEvent(L"FrontGiveWater") = std::bind(&PlayerScript::AttackEffect, playerScript);

		//m_player->GetComponent<Transform>()->SetPosition(Vector2(300.0f, 250.0f));
		//m_player->GetComponent<Transform>()->SetScale(Vector2(0.5f, 0.5f));

		//m_player->AddComponent<Rigidbody>();

		//// 픽셀 맵을 월드 좌표 (0, 0)에 배치한다.
		//Floor* floor = object::Instantiate<Floor>(
		//	enums::LayerType::Floor,
		//	Vector2(0.0f, 0.0f)
		//);

		//floor->SetName(L"Floor");

		//// 화면에 픽셀 맵을 그리는 컴포넌트
		//SpriteRenderer* floorSpriteRenderer =
		//	floor->AddComponent<SpriteRenderer>();

		//graphics::Texture* pixelMapTexture =
		//	Resources::Find<graphics::Texture>(L"PixelMap");

		//floorSpriteRenderer->SetTexture(pixelMapTexture);

		//// 플레이어도 같은 이미지로 발밑의 색을 검사한다.
		//playerScript->SetPixelMapTexture(pixelMapTexture);

		//// 원본대로 바닥의 오디오 컴포넌트와 클립 연결은 유지한다.
		//AudioSource* audioSource =
		//	floor->AddComponent<AudioSource>();

		//AudioClip* audioClip = Resources::Load<AudioClip>(
		//	L"BGSound",
		//	L"..\\Resources\\Sound\\smw_bonus_game_end.wav"
		//);

		//audioSource->SetClip(audioClip);

		// 지금은 재생하지 않고 착지할 때 재생
		
	}

	void PlayScene::Update()
	{
		Scene::Update();
	}

	void PlayScene::LateUpdate()
	{
		Scene::LateUpdate();

		/*if (Input::GetKeyDown(KeyCode::N))
		{
			SceneManager::LoadScene(L"TitleScene");
		}*/
	}

	void PlayScene::Render(HDC hdc)
	{
		Scene::Render(hdc);
	}

	void PlayScene::OnEnter()
	{
		Scene::OnEnter();

		/*CollisionManager::CollisionLayerCheck(
			enums::LayerType::Player,
			enums::LayerType::Animal,
			true);

		CollisionManager::CollisionLayerCheck(
			enums::LayerType::Player,
			enums::LayerType::Floor,
			true);

		UIManager::Push(enums::UIType::Button);*/
	}

	void PlayScene::OnExit()
	{/*
		UIManager::Pop(enums::UIType::Button);*/

		Scene::OnExit();
	}
}