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
			return; // 등록할 객체가 없으면 함수 종료
		}

		m_gameObjects.push_back(gameObject); // 목록 맨 뒤에 주소 추가
	}

}