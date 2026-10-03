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
		// 게임 오브젝트 만들기 전 리소스들 전부 Load하면 좋다
		backGround = object::Instantiate<Player>(enums::LayerType::BackGround, Vector2(100.0f, 100.0f));

		SpriteRenderer* spriteRenderer = backGround->AddComponent<SpriteRenderer>();
		graphics::Texture* backGround = Resources::Find < graphics::Texture>(L"BackGround");
		spriteRenderer->SetTexture(backGround);
		spriteRenderer->SetTexture(backGround);

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