#include "opPlayScene.h"
#include "opGameObject.h"
#include "opPlayer.h"
#include "opTransform.h"
#include "opSpriteRenderer.h"
#include "opInput.h"
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
		backGround = new Player();
		Transform* transform = backGround->AddComponent<Transform>();
		transform->SetPos(Vector2(0, 0));
		transform->SetName(L"Transform");

		SpriteRenderer* spriteRenderer = backGround->AddComponent<SpriteRenderer>();
		spriteRenderer->SetName(L"SpriteRenderer");
		spriteRenderer->ImageLoad(L"C:\\Workspace\\opEngine\\Resources\\CloudOcean.png");
	
		AddGameObject(backGround, LayerType::BackGround);
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
	{
		Transform* transform = backGround->GetComponent<Transform>();
		transform->SetPos(Vector2(0, 0));
	}
}