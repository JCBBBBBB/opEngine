#pragma once
#include "opComponent.h"
#include "opGameObject.h"
#include "opLayer.h"
#include "opScene.h"
#include "opSceneManager.h"
#include "opTransform.h"

namespace op::object
{
	template <typename T>
	static T* Instantiate(op::enums::LayerType layerType)
	{
		T* gameObject = new T();
		Scene activeScene = SceneManager::GetActiveScene();
		Layer* layer = activeScene->GetLayer(layerType);
		layer->AddGameObject(gameObject);

		return gameObject;
	}

	template <typename T>
	static T* Instantiate(op::enums::LayerType layerType, math::Vector2 position)
	{
		T* gameObject = new T();
		Scene* activeScene = SceneManager::GetActiveScene();
		Layer* layer = activeScene->GetLayer(layerType);
		layer->AddGameObject(gameObject);

		Transform* transform = gameObject->GetComponent<Transform>();
		transform->SetPosition(position);

		return gameObject;
	}
}
