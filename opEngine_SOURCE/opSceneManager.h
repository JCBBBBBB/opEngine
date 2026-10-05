#pragma once
#include "opScene.h"

namespace op
{
	class SceneManager
	{
	public:
		// 씬 생성하는 함수
		template <typename T>
		static Scene* CreateScene(const std::wstring& name)
		{
            auto existing = m_scene.find(name);
            if (existing != m_scene.end()) return existing->second;
			T* scene = new T();
			scene->SetName(name);
			m_activeScene = scene;
			scene->Initialize();

			m_scene.insert(std::make_pair(name, scene));

			return scene;
		}

		// 이미 만든 씬을 현재 씬으로 선택하는 함수
		static Scene* LoadScene(const std::wstring& name);
		static Scene* GetActiveScene() { return m_activeScene; }
        static Scene* GetDontDestroyOnLoad() { return m_dontDestroyOnLoad; }
		// 현재 씬과 유지 씬에서 해당 레이어의 객체를 함께 가져온다.
		static std::vector<GameObject*> GetGameObjects(
			enums::LayerType layerType);


		static void Initialize();
		static void Update();
		static void LateUpdate();
		static void Render(HDC hdc);
		static void Release();
		static void Destroy();

	private:
        friend class Layer;
        struct ObjectPass;
        static bool ShouldProcess(GameObject* gameObject);
        static std::unordered_set<GameObject*>* m_processedObjects;
		static std::map<std::wstring, Scene*> m_scene; // 이름으로 찾을 수 있는 씬 목록
		static Scene* m_dontDestroyOnLoad;
		static Scene* m_activeScene; // 현재 실행할 씬
	};
}
