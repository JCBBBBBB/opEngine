#pragma once
#include "CommonInclude.h"
#include "opComponent.h"

namespace op
{
	class GameObject
	{
	public:
		GameObject();
		virtual ~GameObject();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render(HDC hdc);

		template <typename T>
		T* AddComponent()
		{
			T* component = new T();
			component->Initialize();
			component->SetOwner(this);
			m_components[(UINT)component->GetType()] = component;

			return component;
		}

		template<typename T>
		T* GetComponent()
		{
			T* component = nullptr;
			for (Component* comp : m_components)
			{
				component = dynamic_cast<T*>(comp);
				if (component)
				{
					break;
				}
			}

			return component;
		}
	
	private:
		void InitializeTransform();

	private:
		std::vector<Component*> m_components;
	};
}
