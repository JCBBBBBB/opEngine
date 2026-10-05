#pragma once
#include "CommonInclude.h"
#include "opComponent.h"

namespace op::object
{
	void Destroy(op::GameObject* gameObject);
}


namespace op
{

	class GameObject
	{
	public:
		friend void object::Destroy(GameObject* gameObject);

		enum class State
		{
			Active,
			Paused,
			Dead,
			End,
		};

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
			component->SetOwner(this);
            const size_t index = static_cast<size_t>(component->GetType());
            delete m_components[index];
            m_components[index] = component;
            component->Initialize();

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
		
		State GetState() { return m_state; }
		void SetActive(bool power)
		{
			if (power == true)
			{
				m_state = State::Active;
			}
			if (power == false)
			{
				m_state = State::Paused;
			}
		}

		bool IsActive() { return m_state == State::Active; }
		bool IsDead() { return m_state == State::Dead; }

	private:
		void InitializeTransform();
		void Death() { m_state = State::Dead; }
	private:
		std::vector<Component*> m_components;
		State m_state;
	};
}
