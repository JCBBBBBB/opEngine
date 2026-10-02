#include "opScene.h"

namespace op
{
	Scene::Scene() : m_layers{}
	{
		CreateLayers();
	}

	Scene::~Scene()
	{

	}

	void Scene::Initialize()
	{
		for (Layer* layer : m_layers)
		{
			if (layer == nullptr)
			{
				continue;
			}

			layer->Initialize();
		}
	}

	void Scene::Update()
	{
		for (Layer* layer : m_layers)
		{
			if (layer == nullptr)
			{
				continue;
			}

			layer->Update();
		}
	}

	void Scene::LateUpdate()
	{
		for (Layer* layer : m_layers)
		{
			if (layer == nullptr)
			{
				continue;
			}

			layer->LateUpdate();
		}
	}

	void Scene::Render(HDC hdc)
	{
		for (Layer* layer : m_layers)
		{
			if (layer == nullptr)
			{
				continue;
			}

			layer->Render(hdc);
		}
	}

	void Scene::AddGameObject(GameObject* gameObject, const LayerType layerType)
	{
		m_layers[(UINT)layerType]->AddGameObject(gameObject);
	}

	void Scene::CreateLayers()
	{
		m_layers.resize((UINT)LayerType::Max);
		for (size_t i = 0; i < (UINT)LayerType::Max; i++)
		{
			m_layers[i] = new Layer();
		}
	}

	void Scene::OnEnter()
	{

	}

	void Scene::OnExit()
	{

	}

}