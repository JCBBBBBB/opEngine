#pragma once
#include "opEntity.h"

namespace op
{
	class GameObject;
	class Component : public Entity
	{
	public:
		Component();
		virtual ~Component();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render(HDC hdc);

		void SetOwner(GameObject* owner) { m_owner = owner; }
		GameObject* GetOwner() { return m_owner; }

	private:
		GameObject* m_owner; // 어떤 게임 객체에 붙어 있는지 저장
	};
}
