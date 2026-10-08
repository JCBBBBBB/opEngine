#include "opResultScene.h"

#include "../opEngine_SOURCE/opApplication.h"
#include "../opEngine_SOURCE/opInput.h"
#include "../opEngine_SOURCE/opSceneManager.h"

#include <string>

extern op::Application application;

namespace op
{
	ResultScene::ResultScene() : m_score(0), m_stage(1), m_isClear(false)
	{
	}

	ResultScene::~ResultScene()
	{

	}

	void ResultScene::LateUpdate()
	{
		Scene::LateUpdate();

		if (GetForegroundWindow() != application.GetHwnd())
		{
			return;
		}

		if (Input::GetKeyDown(KeyCode::R))
		{
			SceneManager::LoadScene(L"PlayScene");
			return;
		}

		if (Input::GetKeyDown(KeyCode::Enter))
		{
			SceneManager::LoadScene(L"TitleScene");
			return;
		}
	}

	void ResultScene::Render(HDC hdc)
	{
		if (hdc == nullptr)
		{
			return;
		}

		Scene::Render(hdc);

		LONG width = static_cast<LONG>(application.GetWidth());
		LONG height = static_cast<LONG>(application.GetHeight());

		int oldBackgroundMode = SetBkMode(hdc, TRANSPARENT);
		COLORREF oldTextColor = SetTextColor(hdc, RGB(255, 255, 255));

		RECT area = {};
		area.right = width;
		area.top = height / 3;
		area.bottom = area.top + 40;

		const wchar_t* message =
			m_isClear ? L"ALL CLEAR" : L"GAME OVER";

		// 종료 이유를 표시한다.
		DrawTextW(
			hdc, message, -1, &area,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE
		);

		std::wstring result =
			L"Stage : " + std::to_wstring(m_stage)
			+ L"   Score : " + std::to_wstring(m_score);

		area.top += 60;
		area.bottom = area.top + 40;

		// 최종 스테이지와 점수를 표시한다.
		DrawTextW(
			hdc, result.c_str(), -1, &area,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE
		);

		area.top += 60;
		area.bottom = area.top + 40;

		DrawTextW(
			hdc, L"R : 다시 시작", -1, &area,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE
		);

		area.top += 40;
		area.bottom = area.top + 40;

		DrawTextW(
			hdc, L"Enter : 시작 화면", -1, &area,
			DT_CENTER | DT_VCENTER | DT_SINGLELINE
		);

		SetTextColor(hdc, oldTextColor);
		SetBkMode(hdc, oldBackgroundMode);
	}

	void ResultScene::SetResult(int score, int stage, bool isClear)
	{
		m_score = score;
		m_stage = stage;
		m_isClear = isClear;
	}
}