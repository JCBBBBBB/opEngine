#pragma once
#include "CommonInclude.h"
#include "opUIBase.h"

namespace op
{
	class UIManager
	{
	public:
		static void Initialize();

		// 종류에 맞는 UI를 찾아서 열기
		static void OnLoad(enums::UIType type);

		static void Update();
		static void LateUpdate();
		static void Render(HDC hdc);

		// UI를 찾았을 때 / 찾지 못했을 때
		static void OnComplete(UIBase* addUI);
		static void OnFail();

		// UI 열기 요청 / UI 제거
		static void Push(enums::UIType type);
		static void Pop(enums::UIType type);

		static void Release();

	private:
		// 종류별로 등록된 UI 목록
		static std::unordered_map<enums::UIType, UIBase*> m_UIs;

		// 현재 열려 있는 UI 목록
		static std::stack<UIBase*> m_UIBases;

		// 앞으로 열 UI의 요청 목록
		static std::queue<enums::UIType> m_requestUIQueue;

		// 이번 단계에서는 실제로 활용되지 않는 변수
		static UIBase* m_activeUI;
	};
}