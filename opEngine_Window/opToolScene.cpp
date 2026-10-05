#include "opToolScene.h"
#include "opTile.h"

#include "..\\opEngine_SOURCE\\opInput.h"
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

		// 마우스 왼쪽 버튼을 처음 누른 프레임에만 생성
		if (!Input::GetKeyDown(KeyCode::LButton))
		{
			return;
		}

		// 메인 창에서 클릭했을 때만 배치한다.
		if (GetForegroundWindow() != application.GetHwnd())
		{
			return;
		}

		Vector2 pos = Input::GetMousePosition();

		// 메인 창의 그리기 영역 밖에서는 생성하지 않는다.
		if (pos.x < 0.0f || pos.y < 0.0f
			|| pos.x >= static_cast<float>(application.GetWidth())
			|| pos.y >= static_cast<float>(application.GetHeight()))
		{
			return;
		}

		// 마우스 픽셀 좌표를 격자의 열과 행으로 변환
		const int indexX = static_cast<int>(
			pos.x / TilemapRenderer::TileSize.x);

		const int indexY = static_cast<int>(
			pos.y / TilemapRenderer::TileSize.y);

		Tile* tile = object::Instantiate<Tile>(
			enums::LayerType::Tile);

		TilemapRenderer* tileRenderer =
			tile->AddComponent<TilemapRenderer>();

		tileRenderer->SetTexture(
			Resources::Find<graphics::Texture>(L"SpringFloor"));

		// 해당 격자 칸의 왼쪽 위에 배치
		tile->SetPosition(indexX, indexY);
	}

	void ToolScene::Render(HDC hdc)
	{
		Scene::Render(hdc);

		// 세로선
		for (int i = 0; i < 50; ++i)
		{
			const int x = static_cast<int>(
				TilemapRenderer::TileSize.x * static_cast<float>(i));

			MoveToEx(hdc, x, 0, nullptr);
			LineTo(hdc, x, 1000);
		}

		// 가로선
		for (int i = 0; i < 50; ++i)
		{
			const int y = static_cast<int>(
				TilemapRenderer::TileSize.y * static_cast<float>(i));

			MoveToEx(hdc, 0, y, nullptr);
			LineTo(hdc, 1000, y);
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

LRESULT CALLBACK WndTileProc(
	HWND hWnd,
	UINT message,
	WPARAM wParam,
	LPARAM lParam)
{
	switch (message)
	{
	case WM_LBUTTONDOWN:
	{
		// 이번 커밋에서는 시트에서 타일을 선택하는 기능은 아직 없다.
	}
	break;

	case WM_PAINT:
	{
		PAINTSTRUCT ps = {};
		HDC hdc = BeginPaint(hWnd, &ps);

		op::graphics::Texture* texture =
			op::Resources::Find<op::graphics::Texture>(L"SpringFloor");

		if (texture && texture->GetHdc())
		{
			const int width = static_cast<int>(texture->GetWidth());
			const int height = static_cast<int>(texture->GetHeight());

			// 원본 타일 이미지 전체를 원래 크기로 표시
			TransparentBlt(
				hdc,
				0, 0,
				width, height,
				texture->GetHdc(),
				0, 0,
				width, height,
				RGB(255, 0, 255));
		}

		EndPaint(hWnd, &ps);
	}
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}