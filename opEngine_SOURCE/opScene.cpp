#include "opScene.h"

namespace op
{
	Scene::Scene() : m_gameObjects{}
	{

	}

	Scene::~Scene()
	{

	}

	void Scene::Initialize()
	{

	}

	void Scene::Update()
	{
		for (GameObject* gameObject : m_gameObjects)
		{
			gameObject->Update();
		}
	}

	void Scene::LateUpdate()
	{
		for (GameObject* gameObject : m_gameObjects)
		{
			gameObject->LateUpdate();
		}
	}

	void Scene::Render(HDC hdc)
	{
		for (GameObject* gameObject : m_gameObjects)
		{
			gameObject->Render(hdc);
		}
	}

	void Scene::AddGameObject(GameObject* gameObject)
	{
		m_gameObjects.push_back(gameObject);
	}

}