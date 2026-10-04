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
			T* scene = new T();
			scene->SetName(name);
			m_activeScene = scene;
			scene->Initialize();

			m_scene.insert(std::make_pair(name, scene));

			return scene;
		}

		// 이미 만든 씬을 현재 씬으로 선택하는 함수
		static Scene* LoadScene(const std::wstring& name)
		{
			if (m_activeScene)
			{
				m_activeScene->OnExit();
			}

			std::map<std::wstring, Scene*>::iterator iter = m_scene.find(name);
			if (iter == m_scene.end())
			{
				return nullptr;
			}

			m_activeScene = iter->second;
			m_activeScene->OnEnter();

			return iter->second;
		}
		static Scene* GetActiveScene() { return m_activeScene; }

		static void Initialize();
		static void Update();
		static void LateUpdate();
		static void Render(HDC hdc);
		static void Release();


	private:
		static std::map<std::wstring, Scene*> m_scene; // 이름으로 찾을 수 있는 씬 목록
		static Scene* m_activeScene; // 현재 실행할 씬
	};
}
