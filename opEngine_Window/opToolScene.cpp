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

		if (!Input::GetKeyDown(KeyCode::LButton))
		{
			return;
		}

		if (GetForegroundWindow() != application.GetHwnd())
		{
			return;
		}

		Vector2 pos = Input::GetMousePosition();

		if (pos.x < 0.0f || pos.y < 0.0f
			|| pos.x >= static_cast<float>(application.GetWidth())
			|| pos.y >= static_cast<float>(application.GetHeight()))
		{
			return;
		}

		if (TilemapRenderer::SelectedIndex.x < 0.0f
			|| TilemapRenderer::SelectedIndex.y < 0.0f)
		{
			return;
		}

		// 메인 창에서 배치할 격자 위치
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

		// 어떤 그림을 사용할지 설정
		tileRenderer->SetIndex(TilemapRenderer::SelectedIndex);

		// 메인 창의 어느 칸에 배치할지 설정
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
		POINT mousePos = {};

		// 모니터 좌표를 타일용 창 내부 좌표로 변환
		GetCursorPos(&mousePos);
		ScreenToClient(hWnd, &mousePos);

		op::graphics::Texture* texture =
			op::Resources::Find<op::graphics::Texture>(L"SpringFloor");

		if (!texture)
		{
			break;
		}

		const float tileWidth =
			op::TilemapRenderer::OriginTileSize.x;

		const float tileHeight =
			op::TilemapRenderer::OriginTileSize.y;

		if (tileWidth <= 0.0f || tileHeight <= 0.0f)
		{
			break;
		}

		// 창을 늘렸더라도 이미지 바깥은 선택하지 않는다.
		if (mousePos.x < 0 || mousePos.y < 0
			|| mousePos.x >= static_cast<LONG>(texture->GetWidth())
			|| mousePos.y >= static_cast<LONG>(texture->GetHeight()))
		{
			break;
		}

		// 원본 타일 크기인 16픽셀로 나누어 열과 행을 구한다.
		const int indexX = static_cast<int>(
			static_cast<float>(mousePos.x) / tileWidth);

		const int indexY = static_cast<int>(
			static_cast<float>(mousePos.y) / tileHeight);

		// 선택한 타일 번호 저장
		op::TilemapRenderer::SelectedIndex = op::math::Vector2(
			static_cast<float>(indexX),
			static_cast<float>(indexY));
	}
	break;
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