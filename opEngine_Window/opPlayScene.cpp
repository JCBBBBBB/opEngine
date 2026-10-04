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

		// 플레이어
		m_player = object::Instantiate<Player>(enums::LayerType::Player);
		//SpriteRenderer* spriteRenderer = m_player->AddComponent<SpriteRenderer>();
		//spriteRenderer->SetSize(Vector2(3.0f, 3.0f)); // 이미지를 그릴 크기의 배율
		m_player->AddComponent<PlayerScript>(); // 방향키 이동 기능 붙인다

		graphics::Texture* chickenTexture = Resources::Find<graphics::Texture>(L"Cat"); // Resources 목록에서 chicken 찾기
		Animator* animator = m_player->AddComponent<Animator>();
		animator->CreateAnimation(L"CatFrontMove", chickenTexture, Vector2(0.0f, 0.0f), Vector2(32.0f, 32.0f), Vector2::Zero, 4, 0.5f); // 애니메이션 이름, 원본 이미지, 첫 프레임 시작 위치, 프레임 한 장의 크기, 보정값, 가로로 이어진 프레임 개수, 프레임당 유지시간
		animator->PlayAnimation(L"CatFrontMove", true); // 0.5초마다 다음 프레임으로 바뀌므로 한 번의 재생은 약 2초이다, true는 반복 재생
		/*spriteRenderer->SetTexture(chickenTexture);*/

		// 배경
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
	{

	}
}