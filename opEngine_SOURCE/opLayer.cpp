#include "opLayer.h"
#include "opSceneManager.h"

namespace op
{
	Layer::Layer() : m_gameObjects{}
	{

	}

	Layer::~Layer()
	{
		for (GameObject* gameObject : m_gameObjects)
		{
			if (gameObject == nullptr)
			{
				continue;
			}

			delete gameObject;
			gameObject = nullptr;
		}
	}

	void Layer::Initialize()
	{
        // 실행 중 씬 이동/객체 생성으로 원본 vector가 바뀌어도 반복자는 안전하다.
        const auto objects = m_gameObjects;
        for (GameObject* gameObject : objects)
        {
            if (!gameObject) continue;
            if (std::find(m_gameObjects.begin(), m_gameObjects.end(), gameObject) == m_gameObjects.end()) continue;
            gameObject->Initialize();
        }
	}

	void Layer::Update()
	{
        // 실행 중 씬 이동/객체 생성으로 원본 vector가 바뀌어도 반복자는 안전하다.
        const auto objects = m_gameObjects;
        for (GameObject* gameObject : objects)
        {
            if (!gameObject || !gameObject->IsActive()) continue;
            if (std::find(m_gameObjects.begin(), m_gameObjects.end(), gameObject) == m_gameObjects.end()) continue;
            if (SceneManager::ShouldProcess(gameObject)) gameObject->Update();
        }
	}

	void Layer::LateUpdate()
	{
        // 실행 중 씬 이동/객체 생성으로 원본 vector가 바뀌어도 반복자는 안전하다.
        const auto objects = m_gameObjects;
        for (GameObject* gameObject : objects)
        {
            if (!gameObject || !gameObject->IsActive()) continue;
            if (std::find(m_gameObjects.begin(), m_gameObjects.end(), gameObject) == m_gameObjects.end()) continue;
            if (SceneManager::ShouldProcess(gameObject)) gameObject->LateUpdate();
        }
	}

	void Layer::Render(HDC hdc)
	{
        // 실행 중 씬 이동/객체 생성으로 원본 vector가 바뀌어도 반복자는 안전하다.
        const auto objects = m_gameObjects;
        for (GameObject* gameObject : objects)
        {
            if (!gameObject || !gameObject->IsActive()) continue;
            if (std::find(m_gameObjects.begin(), m_gameObjects.end(), gameObject) == m_gameObjects.end()) continue;
            if (SceneManager::ShouldProcess(gameObject)) gameObject->Render(hdc);
        }
	}

	void Layer::Destroy()
	{
		std::vector<GameObject*> deleteObjects = {};

		findDeadGameObjects(deleteObjects);
		eraseDeadGameObject();
		deleteGameObjects(deleteObjects);
	}

	void Layer::AddGameObject(GameObject* gameObject)
	{
        if (!gameObject) return;
        if (std::find(m_gameObjects.begin(), m_gameObjects.end(), gameObject) == m_gameObjects.end())
            m_gameObjects.push_back(gameObject);
	}

	void Layer::findDeadGameObjects(OUT std::vector<GameObject*>& gameObjs)
	{
		for (GameObject* gameObject : m_gameObjects)
		{
			if (!gameObject) continue;
			GameObject::State active = gameObject->GetState();

			if (active == GameObject::State::Dead)
			{
				gameObjs.push_back(gameObject);
			}
		}
	}

	void Layer::deleteGameObjects(std::vector<GameObject*>& gameObjs)
	{
		for (GameObject* obj : gameObjs)
		{
			delete obj;
			obj = nullptr;
		}
	}

	void Layer::eraseDeadGameObject()
	{
        std::erase_if(m_gameObjects, [](GameObject* gameObject)
        { return !gameObject || gameObject->IsDead(); });
	}
    void Layer::EraseGameObject(GameObject* gameObject)
    {
        // 등록만 해제한다. 다른 씬으로 옮길 객체이므로 여기서 delete하지 않는다.
        std::erase(m_gameObjects, gameObject);
    }
}