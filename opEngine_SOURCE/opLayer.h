#pragma once
#include "opEntity.h"
#include "CommonInclude.h"
#include "opGameObject.h"

namespace op
{
	class Layer : public Entity
	{
	public:
		Layer();
		~Layer();

		virtual void Initialize();
		virtual void Update();
		virtual void LateUpdate();
		virtual void Render(HDC hdc);
		virtual void Destroy();

		void AddGameObject(GameObject* gameObject);
        void EraseGameObject(GameObject* gameObject);
        const std::vector<GameObject*>& GetGameObjects() const { return m_gameObjects; }

	private:
		void findDeadGameObjects(OUT std::vector<GameObject*>& gameObjs);
		void deleteGameObjects(std::vector<GameObject*>& gameObjs);
		void eraseDeadGameObject();

	private:
		std::vector<GameObject*> m_gameObjects; // 이 Layer에 등록된 오브젝트 목록
	};

	typedef std::vector<GameObject*>::iterator GameObjectIter;
}
