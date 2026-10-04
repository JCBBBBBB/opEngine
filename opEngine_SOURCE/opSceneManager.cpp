#include "opSceneManager.h"

namespace op
{
	std::map<std::wstring, Scene*> SceneManager::m_scene = {};
	Scene* SceneManager::m_activeScene = nullptr; // 현재 실행할 씬

	void SceneManager::Initialize()
	{

	}

	void SceneManager::Update()
	{
		m_activeScene->Update();
	}

	void SceneManager::LateUpdate()
	{
		m_activeScene->LateUpdate();
	}

	void SceneManager::Render(HDC hdc)
	{
		m_activeScene->Render(hdc);
	}

	void SceneManager::Release()
	{
		for (auto& iter : m_scene)
		{
			delete iter.second;
			iter.second = nullptr;
		}
	}

}