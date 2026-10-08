#pragma once
#include "..\\opEngine_SOURCE\\opSceneManager.h"

#include "opPlayScene.h"
#include "opTitleScene.h"
#include "opResultScene.h"
#include "opToolScene.h"-

namespace op
{	
	// 씬을 만들고 초기화
	void LoadScenes()
	{
		SceneManager::CreateScene<TitleScene>(L"TitleScene");
		PlayScene* playScene = static_cast<PlayScene*>(SceneManager::CreateScene<PlayScene>(L"PlayScene"));
		ResultScene* resultScene = static_cast<ResultScene*>(SceneManager::CreateScene<ResultScene>(L"ResultScene"));
		
		playScene->SetResultScene(resultScene); // 플레이 씬에 결과를 전달할 씬을 알려준다

		SceneManager::LoadScene(L"TitleScene"); // PlayScene을 처음 화면으로 선택
	}
}