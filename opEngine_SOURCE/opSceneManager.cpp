#include "opSceneManager.h"
#include "opCollisionManager.h"
#include "opRenderer.h"

namespace op
{
	std::map<std::wstring, Scene*> SceneManager::m_scene = {};
	Scene* SceneManager::m_activeScene = nullptr; // 현재 실행할 씬

	void SceneManager::Initialize()
	{

	}

	void SceneManager::Update()
	{
		if (m_activeScene) m_activeScene->Update();
	}

	void SceneManager::LateUpdate()
	{
		if (m_activeScene) m_activeScene->LateUpdate();
	}

	void SceneManager::Render(HDC hdc)
	{
		if (m_activeScene) m_activeScene->Render(hdc);
	}

	void SceneManager::Release()
	{
		CollisionManager::Reset();
        renderer::mainCamera = nullptr;
		for (auto& iter : m_scene)
		{
			delete iter.second;
			iter.second = nullptr;
		}
		m_scene.clear();
		m_activeScene = nullptr;
	}

	void SceneManager::Destroy()
	{
		if (m_activeScene) m_activeScene->Destroy();
	}
    Scene* SceneManager::LoadScene(const std::wstring& name)
    {
        auto iter = m_scene.find(name);
        if (iter == m_scene.end()) return nullptr;
        CollisionManager::Reset();
        if (m_activeScene) m_activeScene->OnExit();
        m_activeScene = iter->second;
        m_activeScene->OnEnter();
        return m_activeScene;
    }
}