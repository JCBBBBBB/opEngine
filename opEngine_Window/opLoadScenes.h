#pragma once
#include "..\\opEngine_SOURCE\\opSceneManager.h"

#include "opPlayScene.h"

namespace op
{
	void LoadScenes()
	{
		SceneManager::CreateScene<PlayScene>(L"PlayScene");
		//SceneManager::CreateScene<EndScene>(L"EndScene");
		//SceneManager::CreateScene<TitleScene>(L"TitleScene");

		SceneManager::LoadScene(L"PlayScene"); // PlayScene을 현재 씬으로 선택
	}
}