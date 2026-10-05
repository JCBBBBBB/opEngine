#include "opScene.h"

namespace op
{
	Scene::Scene() : m_layers{}
	{
		CreateLayers();
	}

	Scene::~Scene()
	{
		for (Layer* layer : m_layers)
		{
			delete layer;
			layer = nullptr;
		}
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

	void Scene::Destroy()
	{
		for (Layer* layer : m_layers)
		{
			if (layer == nullptr)
				continue;

			layer->Destroy();
		}
	}

	void Scene::AddGameObject(GameObject* gameObject, const enums::LayerType layerType)
	{
        Layer* layer = GetLayer(layerType);
        if (!gameObject || !layer) return;
        Scene* previous = gameObject->m_scene;
        if (previous && (previous != this || gameObject->GetLayerType() != layerType))
            previous->EraseGameObject(gameObject);
        gameObject->SetLayerType(layerType);
        gameObject->m_scene = this;
        layer->AddGameObject(gameObject);
	}

	void Scene::CreateLayers()
	{
		m_layers.resize((UINT)enums::LayerType::Max);
		for (size_t i = 0; i < (UINT)enums::LayerType::Max; i++)
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

    void Scene::EraseGameObject(GameObject* gameObject)
    {
        if (!gameObject || gameObject->m_scene != this) return;
        for (Layer* layer : m_layers)
            if (layer) layer->EraseGameObject(gameObject);
        gameObject->m_scene = nullptr;
    }
}