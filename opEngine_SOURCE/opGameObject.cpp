#include "opGameObject.h"
#include "opInput.h"
#include "opTime.h"
#include "opTransform.h"


namespace op
{

	GameObject::GameObject()
	{
		InitializeTransform();
	}

	GameObject::~GameObject()
	{
		for (Component* component : m_components)
		{
			delete component;
			component = nullptr;
		}
	}
	
	void GameObject::Initialize()
	{
		for (Component* component : m_components)
		{
			component->Initialize();
		}
	}

	void GameObject::Update()
	{
		for (Component* component : m_components)
		{
			component->Update();
		}
	}

	void GameObject::LateUpdate()
	{
		for (Component* component : m_components)
		{
			component->LateUpdate();
		}
	}

	void GameObject::Render(HDC hdc)
	{
		for (Component* component : m_components)
		{
			component->Render(hdc);
		}
	}

	void GameObject::InitializeTransform()
	{
		AddComponent<Transform>();
	}
	
}