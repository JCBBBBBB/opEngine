#include "opTitleScene.h"

#include "../opEngine_SOURCE/opApplication.h"
#include "opInput.h"
#include "opPlayScene.h"
#include "opSceneManager.h"

extern op::Application application;


namespace op
{
	TitleScene::TitleScene()
	{
	}

	TitleScene::~TitleScene()
	{
	}

	void TitleScene::Initialize()
	{
		// 이번 시작 화면은 글자만 그리므로 별도의 게임 오브젝트를 생성하지 않는다
		Scene::Initialize();
	}

	void TitleScene::Update()
	{
		Scene::Update();
	}

	void TitleScene::LateUpdate()
	{
		Scene::LateUpdate();

		// 게임 창을 선택한 상태에서만 시작할 수 있다.
		if (GetForegroundWindow() != application.GetHwnd())
		{
			return;
		}

		if (Input::GetKeyDown(KeyCode::N))
		{
			SceneManager::LoadScene(L"PlayScene");
			return;
		}
	}

	void TitleScene::Render(HDC hdc)
	{
		if (hdc == nullptr)
		{
			return;
		}

		Scene::Render(hdc);

		LONG width = static_cast<LONG>(application.GetWidth());
		LONG height = static_cast<LONG>(application.GetHeight());

		// 글자 배경은 투명하게, 글자 색은 흰색으로 설정한다.
		int oldBackgroundMode = SetBkMode(hdc, TRANSPARENT);
		COLORREF oldTextColor = SetTextColor(hdc, RGB(255, 255, 255));

		// 글자를 배치할 사각형.
		// 가로 범위를 화면 전체로 잡으면 가운데 정렬할 수 있다.
		RECT area = {};
		area.left = 0;
		area.right = width;
		area.top = height / 3;
		area.bottom = area.top + 40;

		DrawTextW(
			hdc,
			L"벽돌깨기",
			-1,
			&area,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE
		);

		// 제목보다 아래쪽에 시작 안내를 표시한다.
		area.top += 70;
		area.bottom = area.top + 40;

		DrawTextW(
			hdc,
			L"Enter : 게임 시작",
			-1,
			&area,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE
		);

		// 패들 조작 안내.
		area.top += 60;
		area.bottom = area.top + 40;

		DrawTextW(
			hdc,
			L"A / D 또는 방향키 좌우 : 패들 이동",
			-1,
			&area,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE
		);

		// 공 발사 안내.
		area.top += 40;
		area.bottom = area.top + 40;

		DrawTextW(
			hdc,
			L"Space : 공 발사",
			-1,
			&area,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE
		);

		// 원래 그리기 설정으로 복원한다.
		SetTextColor(hdc, oldTextColor);
		SetBkMode(hdc, oldBackgroundMode);
	}

    void TitleScene::OnEnter() { Scene::OnEnter(); }
    void TitleScene::OnExit() { Scene::OnExit(); }
}