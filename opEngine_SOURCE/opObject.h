#pragma once
#include "opComponent.h"
#include "opGameObject.h"
#include "opLayer.h"
#include "opScene.h"
#include "opSceneManager.h"
#include "opTransform.h"

namespace op::object
{
	template <typename T> // T 자리에 Player 같은 타입을 전달
	static T* Instantiate(op::enums::LayerType layerType)
	{
		T* gameObject = new T(); // 전달받은 타입의 객체 생성
		Scene* activeScene = SceneManager::GetActiveScene(); // 현재 씬의 주소 가져오기
		Layer* layer = activeScene->GetLayer(layerType); // 그 씬에서 지정한 레이어 가져오기
		layer->AddGameObject(gameObject); // 생성한 오브젝트를 해당 레이어에 등록

		return gameObject;
	}

	template <typename T>
	static T* Instantiate(op::enums::LayerType layerType, math::Vector2 position)
	{
		T* gameObject = new T();
		Scene* activeScene = SceneManager::GetActiveScene();
		Layer* layer = activeScene->GetLayer(layerType);
		layer->AddGameObject(gameObject);

		Transform* transform = gameObject->GetComponent<Transform>(); // 생성한 오브젝트에 붙어 있는 Transform 찾기
		transform->SetPosition(position); // 전달받은 위치를 Transform에 저장

		return gameObject;
	}
}
