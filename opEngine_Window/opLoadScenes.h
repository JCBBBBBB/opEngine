#pragma once
#include "..\\opEngine_SOURCE\\opSceneManager.h"

#include "opPlayScene.h"
#include "opTitleScene.h"

namespace op
{
	void LoadScenes()
	{
		// 시작할 때 두 씬 만들어둔다
		SceneManager::CreateScene<TitleScene>(L"TitleScene");
		SceneManager::CreateScene<PlayScene>(L"PlayScene");

		//SceneManager::CreateScene<EndScene>(L"EndScene");
		//SceneManager::CreateScene<TitleScene>(L"TitleScene");

		SceneManager::LoadScene(L"PlayScene"); // PlayScene을 처음 화면으로 선택
	}
}