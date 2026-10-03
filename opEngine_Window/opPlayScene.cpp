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

namespace op
{
	PlayScene::PlayScene()
	{

	}

	PlayScene::~PlayScene()
	{

	}

	void PlayScene::Initialize()
	{
		// main camera
		GameObject* camera = object::Instantiate<GameObject>(enums::LayerType::None, Vector2(344.0f, 442.0f));
		Camera* cameraComponent = camera->AddComponent<Camera>();
		renderer::mainCamera = cameraComponent;
		//camera->AddComponent<PlayerScript>();

		m_player = object::Instantiate<Player>(enums::LayerType::Player);
		SpriteRenderer* spriteRenderer = m_player->AddComponent<SpriteRenderer>();
		spriteRenderer->SetSize(Vector2(3.0f, 3.0f));
		m_player->AddComponent<PlayerScript>();

		graphics::Texture* packmanTexture = Resources::Find<graphics::Texture>(L"PackMan");
		spriteRenderer->SetTexture(packmanTexture);

		GameObject* backGround = object::Instantiate<GameObject>(enums::LayerType::BackGround);
		SpriteRenderer* backGroundSpriteRenderer = backGround->AddComponent<SpriteRenderer>();
		backGroundSpriteRenderer->SetSize(Vector2(3.0f, 3.0f));

		graphics::Texture* backGroundTexture = Resources::Find<graphics::Texture>(L"Map");
		backGroundSpriteRenderer->SetTexture(backGroundTexture);

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
	{/*
		Transform* transform = backGround->GetComponent<Transform>();
		transform->SetPos(Vector2(0, 0));
	*/}
}