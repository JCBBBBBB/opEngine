#pragma once

#include "opEntity.h"
#include "opGameObject.h"
#include "opLayer.h"

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

		virtual void OnEnter(); // 이 씬을 현재 씬으로 선택했을 때
		virtual void OnExit(); // 현재 씬에서 다른 씬으로 넘어갈 때

		void AddGameObject(GameObject* gameObject, const enums::LayerType layerType);
		Layer* GetLayer(const enums::LayerType type) { return m_layers[(UINT)type]; }

	private:
		void CreateLayers();

	private:
		std::vector<Layer*> m_layers; // 씬에 속한 개임 객체들
	};
}
