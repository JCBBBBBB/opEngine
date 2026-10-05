#include "opLayer.h"

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
		// 목록에 들어있는 오브젝트 주소를 하나씩 가져옴
		for (GameObject* gameObject : m_gameObjects)
		{
			if (gameObject == nullptr)
			{
				continue;
			}

			gameObject->Initialize();
		}
	}

	void Layer::Update()
	{
		for (GameObject* gameObject : m_gameObjects)
		{
			if (gameObject == nullptr)
			{
				continue;
			}

			if (gameObject->IsActive() == false)
			{
				continue;
			}

			gameObject->Update();
		}
	}

	void Layer::LateUpdate()
	{
		for (GameObject* gameObject : m_gameObjects)
		{
			if (gameObject == nullptr)
			{
				continue;
			}

			if (gameObject->IsActive() == false)
			{
				continue;
			}

			gameObject->LateUpdate();
		}
	}

	void Layer::Render(HDC hdc)
	{
		for (GameObject* gameObject : m_gameObjects)
		{
			if (gameObject == nullptr)
			{
				continue;
			}

			if (gameObject->IsActive() == false)
			{
				continue;
			}

			gameObject->Render(hdc);
		}
	}

	void Layer::Destroy()
	{
		std::vector<GameObject*> deleteObjects = {};

		findDeadGameObjects(deleteObjects);
		eraseGameObject();
		deleteGameObjects(deleteObjects);
	}

	void Layer::AddGameObject(GameObject* gameObject)
	{
		if (gameObject == nullptr)
		{
			return; // 등록할 객체가 없으면 함수 종료
		}

		m_gameObjects.push_back(gameObject); // 목록 맨 뒤에 주소 추가
	}

	void Layer::findDeadGameObjects(OUT std::vector<GameObject*>& gameObjs)
	{
		for (GameObject* gameObject : m_gameObjects)
		{
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

	void Layer::eraseGameObject()
	{
		std::erase_if(m_gameObjects, [](GameObject* gameObj) {return (gameObj)->IsDead(); });
	}
}