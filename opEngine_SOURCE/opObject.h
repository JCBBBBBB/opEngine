#pragma once
#include "opComponent.h"
#include "opGameObject.h"
#include "opLayer.h"
#include "opScene.h"
#include "opSceneManager.h"
#include "opTransform.h"
#include <stdexcept>

namespace op::object
{
	template <typename T> // T 자리에 Player 같은 타입을 전달
	static T* Instantiate(op::enums::LayerType layerType)
	{
        Scene* activeScene = SceneManager::GetActiveScene();
        if (!activeScene || !activeScene->GetLayer(layerType))
            throw std::logic_error("Instantiate requires an active scene and a valid layer");
        T* gameObject = new T();
        gameObject->SetLayerType(layerType);
        activeScene->AddGameObject(gameObject, layerType);
        return gameObject;
	}

	template <typename T>
	static T* Instantiate(op::enums::LayerType layerType, math::Vector2 position)
	{
        T* gameObject = Instantiate<T>(layerType);
        if (Transform* transform = gameObject->GetComponent<Transform>())
            transform->SetPosition(position);
        return gameObject;
	}

    void DontDestroyOnLoad(GameObject* gameObject);
}
