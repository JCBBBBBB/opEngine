#include "opLayer.h"

namespace op
{
	Layer::Layer() : m_gameObjects{}
	{

	}

	Layer::~Layer()
	{

	}

	void Layer::Initialize()
	{
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
			gameObject->Render(hdc);
		}
	}

	void Layer::AddGameObject(GameObject* gameObject)
	{
		if (gameObject == nullptr)
		{
			return;
		}

		m_gameObjects.push_back(gameObject);
	}

}