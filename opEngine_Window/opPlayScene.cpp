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
		{
			Player* player = new Player();

			Transform* transform = player->AddComponent<Transform>();
			transform->SetPos(700, 450);
			transform->SetName(L"Transform");

			SpriteRenderer* spriteRenderer = player->AddComponent<SpriteRenderer>();
			spriteRenderer->SetName(L"SpriteRenderer");

			AddGameObject(player);
		}
		{
			Player* player = new Player();

			Transform* transform = player->AddComponent<Transform>();
			transform->SetPos(300, 450);
			transform->SetName(L"Transform");

			SpriteRenderer* spriteRenderer = player->AddComponent<SpriteRenderer>();
			spriteRenderer->SetName(L"SpriteRenderer");

			AddGameObject(player);
		}
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