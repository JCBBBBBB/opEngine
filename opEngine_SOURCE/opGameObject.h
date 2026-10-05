#pragma once
#include "opEntity.h"
#include "CommonInclude.h"
#include "opComponent.h"

namespace op::object
{
	void Destroy(op::GameObject* gameObject);
}


namespace op
{

	class Scene;
	class GameObject : public Entity
	{
	public:
		friend void object::Destroy(GameObject* gameObject);
        friend class Scene;

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
        void SetLayerType(enums::LayerType type) { m_layerType = type; }
        enums::LayerType GetLayerType() const { return m_layerType; }
        Scene* GetScene() const { return m_scene; }

	private:
		void InitializeTransform();
		void Death() { m_state = State::Dead; }
	private:
		std::vector<Component*> m_components;
		State m_state;
        enums::LayerType m_layerType;
        Scene* m_scene; // 한 씬만 소유하게 하여 이전 씬과 중복 삭제되지 않게 한다.
	};
}
