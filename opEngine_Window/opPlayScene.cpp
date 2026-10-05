#include "opTile.h"
#include "..\\opEngine_SOURCE\\opTilemapRenderer.h"
#include <cstdio>

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
#include "opCircleCollider2D.h"
#include "opCollisionManager.h"

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
		// 저장된 타일맵 읽기
		{
			FILE* mapFile = nullptr;

			if (_wfopen_s(
				&mapFile,
				L"..\\Resources\\Test.tile",
				L"rb") != 0 || !mapFile)
			{
				MessageBoxW(
					nullptr,
					L"Resources 폴더에 Test.tile 파일이 없습니다.",
					L"타일맵 불러오기",
					MB_OK | MB_ICONWARNING);
			}
			else
			{
				graphics::Texture* tileTexture =
					Resources::Find<graphics::Texture>(L"SpringFloor");

				if (tileTexture)
				{
					while (true)
					{
						int data[4] = {};

						const size_t readBytes =
							std::fread(data, 1, sizeof(data), mapFile);

						if (readBytes == 0)
						{
							if (std::ferror(mapFile))
							{
								MessageBoxW(
									nullptr,
									L"타일맵을 읽는 중 오류가 발생했습니다.",
									L"타일맵 불러오기",
									MB_OK | MB_ICONERROR);
							}

							break;
						}

						if (readBytes != sizeof(data))
						{
							MessageBoxW(
								nullptr,
								L"타일맵의 데이터가 불완전합니다.",
								L"타일맵 불러오기",
								MB_OK | MB_ICONERROR);
							break;
						}

						Tile* tile = object::Instantiate<Tile>(
							enums::LayerType::Tile,
							Vector2(
								static_cast<float>(data[2]),
								static_cast<float>(data[3])));

						TilemapRenderer* tileRenderer =
							tile->AddComponent<TilemapRenderer>();

						tileRenderer->SetTexture(tileTexture);

						tileRenderer->SetIndex(Vector2(
							static_cast<float>(data[0]),
							static_cast<float>(data[1])));
					}
				}

				std::fclose(mapFile);
			}
		}

        CollisionManager::CollisionLayerCheck(enums::LayerType::Player, enums::LayerType::Animal, true);
		// main camera
		GameObject* camera = object::Instantiate<GameObject>(
			enums::LayerType::Particle,
			Vector2(336.0f, 423.0f));		Camera* cameraComponent = camera->AddComponent<Camera>();
		renderer::mainCamera = cameraComponent;

		// 플레이어
		m_player = object::Instantiate<Player>(enums::LayerType::Player); // 플레이어 생성, 생성자에서 Transform이 자동으로 붙음
        object::DontDestroyOnLoad(m_player);
		CircleCollider2D* collider = m_player->AddComponent<CircleCollider2D>();
        collider->SetOffset(Vector2(-50.0f, -50.0f));
        PlayerScript* playerScript = m_player->AddComponent<PlayerScript>(); // 방향키 이동 기능 붙인다

		graphics::Texture* playerTexture = Resources::Find<graphics::Texture>(L"Player"); // LoadResources()에서 Cat으로 등록한 bmp파일 찾음, Resources 목록에서 chicken 찾기
		Animator* playerAnimator = m_player->AddComponent<Animator>(); // 애니메이션을 관리하고 그리는 컴포넌트 추가
		playerAnimator->CreateAnimation(L"Idle", playerTexture, Vector2(2000.0f, 250.0f), Vector2(250.0f, 250.0f), Vector2::Zero, 1, 0.1f);
		playerAnimator->CreateAnimation(L"FrontGiveWater", playerTexture, Vector2(0.0f, 2000.0f), Vector2(250.0f, 250.0f), Vector2::Zero, 12, 0.1f);

		playerAnimator->PlayAnimation(L"Idle", false); // 처음에는 대기 애니메이션
		playerAnimator->GetCompleteEvent(L"FrontGiveWater") = std::bind(&PlayerScript::AttackEffect, playerScript);

		m_player->GetComponent<Transform>()->SetPosition(Vector2(300.0f, 250.0f));
		m_player->GetComponent<Transform>()->SetScale(Vector2(0.5f, 0.5f));


        // 원본과 같이 Animal 레이어에 버섯 애니메이션을 사용하는 Cat을 생성한다.
        Cat* cat = object::Instantiate<Cat>(enums::LayerType::Animal);
        cat->AddComponent<CatScript>();
        Animator* catAnimator = cat->AddComponent<Animator>();
        CircleCollider2D* catCollider = cat->AddComponent<CircleCollider2D>();
        catCollider->SetOffset(Vector2(-50.0f, -50.0f));
        catAnimator->CreateAnimationByFolder(L"MushroomIdle", L"..\\Resources\\Mushroom", Vector2::Zero, 0.1f);
        catAnimator->PlayAnimation(L"MushroomIdle", true);
        cat->GetComponent<Transform>()->SetPosition(Vector2(200.0f, 200.0f));
        cat->GetComponent<Transform>()->SetScale(Vector2(1.0f, 1.0f));

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
        Scene::OnEnter();
	}

	void PlayScene::OnExit()
	{
        Scene::OnExit();
	}
}