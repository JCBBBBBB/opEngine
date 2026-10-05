#include "opSceneManager.h"
#include "opCollisionManager.h"
#include "opRenderer.h"
#include "opDontDestroyOnLoad.h"

namespace op
{
    std::unordered_set<GameObject*>* SceneManager::m_processedObjects = nullptr;
    // 한 번의 Update/LateUpdate/Render 도중 옮긴 객체도 한 번만 실행한다.
    struct SceneManager::ObjectPass
    {
        std::unordered_set<GameObject*>* previous;
        explicit ObjectPass(std::unordered_set<GameObject*>& objects) : previous(m_processedObjects)
        { m_processedObjects = &objects; }
        ~ObjectPass() { m_processedObjects = previous; }
    };
    bool SceneManager::ShouldProcess(GameObject* gameObject)
    {
        return !m_processedObjects || m_processedObjects->insert(gameObject).second;
    }
	std::map<std::wstring, Scene*> SceneManager::m_scene = {};
	Scene* SceneManager::m_dontDestroyOnLoad = nullptr;
	Scene* SceneManager::m_activeScene = nullptr; // 현재 실행할 씬

	void SceneManager::Initialize()
	{
        if (!m_dontDestroyOnLoad)
            m_dontDestroyOnLoad = CreateScene<DontDestroyOnLoad>(L"DontDestroyOnLoad");
	}

	void SceneManager::Update()
	{
        std::unordered_set<GameObject*> processed;
        ObjectPass pass(processed);
        if (m_activeScene) m_activeScene->Update();
        if (m_dontDestroyOnLoad && m_activeScene != m_dontDestroyOnLoad)
            m_dontDestroyOnLoad->Update();
	}

	void SceneManager::LateUpdate()
	{
        std::unordered_set<GameObject*> processed;
        ObjectPass pass(processed);
        if (m_activeScene) m_activeScene->LateUpdate();
        if (m_dontDestroyOnLoad && m_activeScene != m_dontDestroyOnLoad)
            m_dontDestroyOnLoad->LateUpdate();
	}

	void SceneManager::Render(HDC hdc)
	{
        std::unordered_set<GameObject*> processed;
        ObjectPass pass(processed);
        if (m_activeScene) m_activeScene->Render(hdc);
        if (m_dontDestroyOnLoad && m_activeScene != m_dontDestroyOnLoad)
            m_dontDestroyOnLoad->Render(hdc);
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
        m_dontDestroyOnLoad = nullptr;
		m_activeScene = nullptr;
	}

	void SceneManager::Destroy()
	{
        if (m_activeScene) m_activeScene->Destroy();
        // 초기화 직후 현재 씬 자체가 유지 씬일 때는 중복 실행하지 않는다.
        if (m_dontDestroyOnLoad && m_activeScene != m_dontDestroyOnLoad)
            m_dontDestroyOnLoad->Destroy();
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

	std::vector<GameObject*> SceneManager::GetGameObjects(
		enums::LayerType layerType)
	{
		std::vector<GameObject*> gameObjects;

		// 현재 씬의 해당 레이어
		if (m_activeScene)
		{
			Layer* layer = m_activeScene->GetLayer(layerType);

			if (layer)
			{
				const auto& objects = layer->GetGameObjects();

				gameObjects.insert(
					gameObjects.end(),
					objects.begin(),
					objects.end());
			}
		}

		// 유지 씬의 해당 레이어
		// 현재 씬이 유지 씬 자체라면 중복으로 추가하지 않는다.
		if (m_dontDestroyOnLoad
			&& m_dontDestroyOnLoad != m_activeScene)
		{
			Layer* layer = m_dontDestroyOnLoad->GetLayer(layerType);

			if (layer)
			{
				const auto& objects = layer->GetGameObjects();

				gameObjects.insert(
					gameObjects.end(),
					objects.begin(),
					objects.end());
			}
		}

		return gameObjects;
	}
}