#include "opPlayScene.h"
#include "opGameObject.h"
#include "opPlayer.h"
#include "opTransform.h"
#include "opSpriteRenderer.h"


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
		Player* backGround = new Player();
		Transform* transform = backGround->AddComponent<Transform>();
		transform->SetPos(Vector2(0, 0));
		transform->SetName(L"Transform");

		SpriteRenderer* spriteRenderer = backGround->AddComponent<SpriteRenderer>();
		spriteRenderer->SetName(L"SpriteRenderer");
		spriteRenderer->ImageLoad(L"C:\\Workspace\\opEngine\\Resources\\CloudOcean.png");
	
		AddGameObject(backGround);
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
}