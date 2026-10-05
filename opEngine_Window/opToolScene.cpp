#include "opToolScene.h"
#include "opTile.h"

#include "..\\opEngine_SOURCE\\opObject.h"
#include "..\\opEngine_SOURCE\\opTilemapRenderer.h"
#include "..\\opEngine_SOURCE\\opResources.h"
#include "..\\opEngine_SOURCE\\opTexture.h"
#include "..\\opEngine_SOURCE\\opCamera.h"
#include "..\\opEngine_SOURCE\\opRenderer.h"
#include "..\\opEngine_SOURCE\\opApplication.h"

extern op::Application application;

namespace op
{
	ToolScene::ToolScene()
	{
	}

	ToolScene::~ToolScene()
	{
	}

	void ToolScene::Initialize()
	{
		// 카메라 중심을 화면 중심과 맞춘다.
		Vector2 cameraPosition(
			static_cast<float>(application.GetWidth()) / 2.0f,
			static_cast<float>(application.GetHeight()) / 2.0f);

		GameObject* camera = object::Instantiate<GameObject>(
			enums::LayerType::Particle,
			cameraPosition);

		Camera* cameraComponent = camera->AddComponent<Camera>();
		renderer::mainCamera = cameraComponent;

		// Tile 레이어에 타일 한 개 생성
		Tile* tile = object::Instantiate<Tile>(
			enums::LayerType::Tile);

		// 원본 시트의 타일 한 칸을 그리는 컴포넌트
		TilemapRenderer* tileRenderer =
			tile->AddComponent<TilemapRenderer>();

		tileRenderer->SetTexture(
			Resources::Find<graphics::Texture>(L"SpringFloor"));

		Scene::Initialize();
	}

	void ToolScene::Update()
	{
		Scene::Update();
	}

	void ToolScene::LateUpdate()
	{
		Scene::LateUpdate();
	}

	void ToolScene::Render(HDC hdc)
	{
		Scene::Render(hdc);

		// 16픽셀 타일을 3배로 표시하므로 격자 간격은 48픽셀
		constexpr int gridSize = 16 * 3;

		// 세로선
		for (int i = 0; i < 50; ++i)
		{
			MoveToEx(hdc, gridSize * i, 0, nullptr);
			LineTo(hdc, gridSize * i, 1000);
		}

		// 가로선
		for (int i = 0; i < 50; ++i)
		{
			MoveToEx(hdc, 0, gridSize * i, nullptr);
			LineTo(hdc, 1000, gridSize * i);
		}
	}

	void ToolScene::OnEnter()
	{
		Scene::OnEnter();
	}

	void ToolScene::OnExit()
	{
		Scene::OnExit();
	}
}