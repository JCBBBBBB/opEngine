#include "opCameraScript.h"
#include "..\\opEngine_SOURCE\\opTransform.h"
#include <commdlg.h>
#include <cstdio>

#pragma comment(lib, "Comdlg32.lib")

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
		Vector2 cameraPosition(
			static_cast<float>(application.GetWidth()) / 2.0f,
			static_cast<float>(application.GetHeight()) / 2.0f);

		GameObject* camera = object::Instantiate<GameObject>(
			enums::LayerType::Particle,
			cameraPosition);

		Camera* cameraComponent = camera->AddComponent<Camera>();

		// 추가: 방향키로 카메라 이동
		camera->AddComponent<CameraScript>();

		renderer::mainCamera = cameraComponent;

		// 기존의 초기 테스트 타일 생성 부분은 제거한다.
		Scene::Initialize();
	}

	void ToolScene::Update()
	{
		Scene::Update();
	}

	void ToolScene::LateUpdate()
	{
		Scene::LateUpdate();

		if (GetForegroundWindow() != application.GetHwnd())
		{
			return;
		}

		if (Input::GetKeyDown(KeyCode::S))
		{
			Save();
			return;
		}

		if (Input::GetKeyDown(KeyCode::L))
		{
			Load();
			return;
		}

		if (Input::GetKeyDown(KeyCode::LButton))
		{
			CreateTileObject();
		}
	}

	void ToolScene::Render(HDC hdc)
	{
		Scene::Render(hdc);

		RenderGreed(hdc);
	}

	void ToolScene::OnEnter()
	{
		Scene::OnEnter();
	}

	void ToolScene::OnExit()
	{
		Scene::OnExit();
	}

	void ToolScene::Save()
	{
		wchar_t filePath[MAX_PATH] = {};

		OPENFILENAMEW ofn = {};
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = application.GetHwnd();
		ofn.lpstrFile = filePath;
		ofn.nMaxFile = static_cast<DWORD>(_countof(filePath));
		ofn.lpstrFilter = L"Tile Files (*.tile)\0*.tile\0\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrDefExt = L"tile";

		// 새 파일 저장 허용, 덮어쓰기 확인, 현재 작업 폴더 유지
		ofn.Flags =
			OFN_PATHMUSTEXIST
			| OFN_OVERWRITEPROMPT
			| OFN_NOCHANGEDIR;

		// 사용자가 취소하면 종료
		if (!GetSaveFileNameW(&ofn))
		{
			return;
		}

		FILE* file = nullptr;

		// wb: 바이너리 쓰기. 기존 파일이면 내용을 덮어쓴다.
		if (_wfopen_s(&file, filePath, L"wb") != 0 || !file)
		{
			MessageBoxW(
				application.GetHwnd(),
				L"저장할 파일을 열 수 없습니다.",
				L"타일맵 저장",
				MB_OK | MB_ICONERROR);
			return;
		}

		bool success = true;

		for (Tile* tile : m_tiles)
		{
			if (!tile)
			{
				continue;
			}

			TilemapRenderer* tileRenderer =
				tile->GetComponent<TilemapRenderer>();

			Transform* transform =
				tile->GetComponent<Transform>();

			if (!tileRenderer || !transform)
			{
				continue;
			}

			Vector2 sourceIndex = tileRenderer->GetIndex();
			Vector2 position = transform->GetPosition();

			// 원본과 같은 순서: 그림 열, 그림 행, 위치 x, 위치 y
			int data[4] = {
				static_cast<int>(sourceIndex.x),
				static_cast<int>(sourceIndex.y),
				static_cast<int>(position.x),
				static_cast<int>(position.y)
			};

			if (std::fwrite(data, sizeof(int), 4, file) != 4)
			{
				success = false;
				break;
			}
		}

		if (std::fclose(file) != 0)
		{
			success = false;
		}

		if (!success)
		{
			MessageBoxW(
				application.GetHwnd(),
				L"파일에 타일 정보를 모두 저장하지 못했습니다.",
				L"타일맵 저장",
				MB_OK | MB_ICONERROR);
		}
	}

	void ToolScene::Load()
	{
		wchar_t filePath[MAX_PATH] = {};

		OPENFILENAMEW ofn = {};
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = application.GetHwnd();
		ofn.lpstrFile = filePath;
		ofn.nMaxFile = static_cast<DWORD>(_countof(filePath));
		ofn.lpstrFilter = L"Tile Files (*.tile)\0*.tile\0\0";
		ofn.nFilterIndex = 1;
		ofn.lpstrDefExt = L"tile";
		ofn.Flags =
			OFN_PATHMUSTEXIST
			| OFN_FILEMUSTEXIST
			| OFN_NOCHANGEDIR;

		if (!GetOpenFileNameW(&ofn))
		{
			return;
		}

		graphics::Texture* texture =
			Resources::Find<graphics::Texture>(L"SpringFloor");

		if (!texture)
		{
			MessageBoxW(
				application.GetHwnd(),
				L"SpringFloor 텍스처가 없습니다.",
				L"타일맵 불러오기",
				MB_OK | MB_ICONERROR);
			return;
		}

		FILE* file = nullptr;

		// rb: 바이너리 읽기
		if (_wfopen_s(&file, filePath, L"rb") != 0 || !file)
		{
			MessageBoxW(
				application.GetHwnd(),
				L"불러올 파일을 열 수 없습니다.",
				L"타일맵 불러오기",
				MB_OK | MB_ICONERROR);
			return;
		}

		while (true)
		{
			int data[4] = {};

			// 타일 한 개의 정보를 읽는다.
			const size_t readBytes =
				std::fread(data, 1, sizeof(data), file);

			if (readBytes == 0)
			{
				if (std::ferror(file))
				{
					MessageBoxW(
						application.GetHwnd(),
						L"파일을 읽는 중 오류가 발생했습니다.",
						L"타일맵 불러오기",
						MB_OK | MB_ICONERROR);
				}

				// 파일 끝 또는 읽기 오류
				break;
			}

			if (readBytes != sizeof(data))
			{
				MessageBoxW(
					application.GetHwnd(),
					L"타일 정보가 불완전합니다.",
					L"타일맵 불러오기",
					MB_OK | MB_ICONERROR);
				break;
			}

			const int indexX = data[0];
			const int indexY = data[1];
			const int posX = data[2];
			const int posY = data[3];

			// 저장한 위치는 이미 픽셀 좌표이므로 그대로 적용한다.
			Tile* tile = object::Instantiate<Tile>(
				enums::LayerType::Tile,
				Vector2(
					static_cast<float>(posX),
					static_cast<float>(posY)));

			TilemapRenderer* tileRenderer =
				tile->AddComponent<TilemapRenderer>();

			tileRenderer->SetTexture(texture);

			tileRenderer->SetIndex(Vector2(
				static_cast<float>(indexX),
				static_cast<float>(indexY)));

			// 불러온 타일도 다음 저장에 포함한다.
			m_tiles.push_back(tile);
		}

		std::fclose(file);
	}

	void ToolScene::CreateTileObject()
	{
		Vector2 pos = Input::GetMousePosition();

		// 메인 창 밖에서 클릭했으면 생성하지 않는다.
		if (pos.x < 0.0f || pos.y < 0.0f)
		{
			return;
		}

		if (!renderer::mainCamera)
		{
			return;
		}

		// 화면 좌표를 게임 좌표로 변환
		pos = renderer::mainCamera->CalculateTilePosition(pos);

		if (pos.x < 0.0f || pos.y < 0.0f)
		{
			return;
		}

		if (TilemapRenderer::SelectedIndex.x < 0.0f
			|| TilemapRenderer::SelectedIndex.y < 0.0f)
		{
			return;
		}

		// 배치할 격자의 열과 행
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

		tileRenderer->SetIndex(TilemapRenderer::SelectedIndex);

		tile->SetIndexPosition(indexX, indexY);

		m_tiles.push_back(tile);
	}

	void ToolScene::RenderGreed(HDC hdc)
	{
		if (!hdc || !renderer::mainCamera)
		{
			return;
		}

		// 세로선
		for (int i = 0; i < 50; ++i)
		{
			Vector2 pos(
				TilemapRenderer::TileSize.x * static_cast<float>(i),
				0.0f);

			pos = renderer::mainCamera->CalculatePosition(pos);

			const int x = static_cast<int>(pos.x);

			MoveToEx(hdc, x, 0, nullptr);
			LineTo(hdc, x, 1000);
		}

		// 가로선
		for (int i = 0; i < 50; ++i)
		{
			Vector2 pos(
				0.0f,
				TilemapRenderer::TileSize.y * static_cast<float>(i));

			pos = renderer::mainCamera->CalculatePosition(pos);

			const int y = static_cast<int>(pos.y);

			MoveToEx(hdc, 0, y, nullptr);
			LineTo(hdc, 1000, y);
		}
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