#include "opGameObject.h"
#include "opInput.h"
#include "opTime.h"
#include "opTransform.h"


namespace op
{

	GameObject::GameObject() : m_state(State::Active)
	{
		m_components.resize((UINT)enums::ComponentType::End);
		InitializeTransform();
	}

	GameObject::~GameObject()
	{
		for (Component* component : m_components)
		{
			if (component == nullptr)
			{
				continue;
			}

			delete component;
			component = nullptr;
		}
	}
	
	void GameObject::Initialize()
	{
		for (Component* component : m_components)
		{
			if (component == nullptr)
			{
				continue;
			}

			component->Initialize();
		}
	}

	void GameObject::Update()
	{
		for (Component* component : m_components)
		{
			if (component == nullptr)
			{
				continue;
			}

			component->Update();
		}
	}

	void GameObject::LateUpdate()
	{
		for (Component* component : m_components)
		{
			if (component == nullptr)
			{
				continue;
			}

			component->LateUpdate();
		}
	}

	void GameObject::Render(HDC hdc)
	{
		for (Component* component : m_components)
		{
			if (component == nullptr)
			{
				continue;
			}

			component->Render(hdc);
		}
	}

	void GameObject::InitializeTransform()
	{
		AddComponent<Transform>();
	}
	
}