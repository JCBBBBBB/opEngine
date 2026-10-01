#pragma once

#include "opEntity.h"
#include "opGameObject.h"

namespace op
{
	class Scene : public Entity
	{
	public:
		Scene();
		~Scene();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render(HDC hdc);

		void AddGameObject(GameObject* gameObject);

	private:
		std::vector<GameObject*> m_gameObjects; // æ¿ø° º”«— ∞≥¿” ∞¥√ºµÈ
	};
}
