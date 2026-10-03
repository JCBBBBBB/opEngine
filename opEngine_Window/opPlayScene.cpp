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
		backGround = object::Instantiate<Player>(enums::LayerType::BackGround, Vector2(100.0f, 100.0f));

		SpriteRenderer* spriteRenderer = backGround->AddComponent<SpriteRenderer>();
		spriteRenderer->ImageLoad(L"C:\\Workspace\\opEngine\\Resources\\CloudOcean.png");
	
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
		wchar_t str[50] = L"PlayScene";
		TextOut(hdc, 0, 0, str, 10);
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