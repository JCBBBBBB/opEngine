#include "opCat.h"
#include "opCatScript.h"

#include "opPlayerScript.h"
#include "opCamera.h"
#include "opRenderer.h"


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

namespace op
{
	PlayScene::PlayScene() : m_player(nullptr)
	{

	}

	PlayScene::~PlayScene()
	{

	}

	void PlayScene::Initialize()
	{
		// main camera
		GameObject* camera = object::Instantiate<GameObject>(enums::LayerType::Particle, Vector2(344.0f, 442.0f));
		Camera* cameraComponent = camera->AddComponent<Camera>();
		renderer::mainCamera = cameraComponent;

		// 플레이어
		m_player = object::Instantiate<Player>(enums::LayerType::Particle); // 플레이어 생성, 생성자에서 Transform이 자동으로 붙음
		PlayerScript* playerScript = m_player->AddComponent<PlayerScript>(); // 방향키 이동 기능 붙인다 

		graphics::Texture* playerTexture = Resources::Find<graphics::Texture>(L"Player"); // LoadResources()에서 Cat으로 등록한 bmp파일 찾음, Resources 목록에서 chicken 찾기
		Animator* playerAnimator = m_player->AddComponent<Animator>(); // 애니메이션을 관리하고 그리는 컴포넌트 추가
		playerAnimator->CreateAnimation(L"Idle", playerTexture, Vector2(2000.0f, 250.0f), Vector2(250.0f, 250.0f), Vector2::Zero, 1, 0.1f);
		playerAnimator->CreateAnimation(L"FrontGiveWater", playerTexture, Vector2(0.0f, 2000.0f), Vector2(250.0f, 250.0f), Vector2::Zero, 12, 0.1f);

		playerAnimator->PlayAnimation(L"Idle", false); // 처음에는 대기 애니메이션
		playerAnimator->GetCompleteEvent(L"FrontGiveWater") = std::bind(&PlayerScript::AttackEffect, playerScript);

		m_player->GetComponent<Transform>()->SetPosition(Vector2(300.0f, 250.0f));
		m_player->GetComponent<Transform>()->SetScale(Vector2(0.5f, 0.5f));


		Scene::Initialize();
	}

	void PlayScene::Update()
	{
		Scene::Update();
	}

	void PlayScene::LateUpdate()
	{
		Scene::LateUpdate();

		if (Input::GetKeyDown(KeyCode::N))
		{
			SceneManager::LoadScene(L"TitleScene");
		}
	}

	void PlayScene::Render(HDC hdc)
	{
		Scene::Render(hdc);
	}

	void PlayScene::OnEnter()
	{

	}

	void PlayScene::OnExit()
	{

	}
}