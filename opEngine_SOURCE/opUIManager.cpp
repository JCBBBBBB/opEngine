
#include "opUIHUD.h"
#include "opUIButton.h"
#include "opUIManager.h"

namespace op
{
	std::unordered_map<enums::UIType, UIBase*> UIManager::m_UIs = {};
	std::stack<UIBase*> UIManager::m_UIBases = {};
	std::queue<enums::UIType> UIManager::m_requestUIQueue = {};
	UIBase* UIManager::m_activeUI = nullptr;

	void UIManager::Initialize()
	{
		// 사용할 UI를 만들어 종류별로 등록
		UIHUD* hud = new UIHUD();
		m_UIs.insert(std::make_pair(enums::UIType::HpBar, hud));

		UIButton* button = new UIButton();
		m_UIs.insert(std::make_pair(enums::UIType::Button, button));
	}

	void UIManager::OnLoad(enums::UIType type)
	{
		auto iter = m_UIs.find(type);

		if (iter == m_UIs.end())
		{
			OnFail();
			return;
		}

		OnComplete(iter->second);
	}

	void UIManager::Update()
	{
		// 원본 목록은 유지하고, 복사한 목록으로 순회
		std::stack<UIBase*> uiBases = m_UIBases;

		while (!uiBases.empty())
		{
			UIBase* uiBase = uiBases.top();
			uiBases.pop();

			if (uiBase)
			{
				uiBase->Update();
			}
		}

		// 한 번의 Update에서 요청 하나 처리
		if (!m_requestUIQueue.empty())
		{
			enums::UIType requestUI = m_requestUIQueue.front();
			m_requestUIQueue.pop();

			OnLoad(requestUI);
		}
	}

	void UIManager::LateUpdate()
	{
		std::stack<UIBase*> uiBases = m_UIBases;

		while (!uiBases.empty())
		{
			UIBase* uiBase = uiBases.top();
			uiBases.pop();

			if (uiBase)
			{
				uiBase->LateUpdate();
			}
		}
	}

	void UIManager::Render(HDC hdc)
	{
		if (m_UIBases.empty())
		{
			return;
		}

		std::vector<UIBase*> buffer;

		// 스택에서 나중에 열린 UI부터 꺼내 임시 저장
		while (!m_UIBases.empty())
		{
			UIBase* uiBase = m_UIBases.top();
			m_UIBases.pop();

			buffer.push_back(uiBase);
		}

		// 먼저 열린 UI부터 처리하도록 순서를 뒤집음
		std::reverse(buffer.begin(), buffer.end());

		for (UIBase* ui : buffer)
		{
			if (ui)
			{
				ui->Render(hdc);
			}

			// 열린 UI 목록을 원래 상태로 복구
			m_UIBases.push(ui);
		}
	}

	void UIManager::OnComplete(UIBase* addUI)
	{
		if (addUI == nullptr)
		{
			return;
		}

		addUI->Initialize();
		addUI->Active();
		addUI->Update();

		// 전체 화면 UI를 열면 기존 UI에 비활성화 요청
		if (addUI->IsFullScreen())
		{
			std::stack<UIBase*> uiBases = m_UIBases;

			while (!uiBases.empty())
			{
				UIBase* uiBase = uiBases.top();
				uiBases.pop();

				if (uiBase)
				{
					uiBase->InActive();
				}
			}
		}

		// 열린 UI 목록에 추가
		m_UIBases.push(addUI);
		m_activeUI = nullptr;
	}

	void UIManager::OnFail()
	{
		m_activeUI = nullptr;
	}

	void UIManager::Push(enums::UIType type)
	{
		// 바로 열지 않고 요청 목록에 추가
		m_requestUIQueue.push(type);
	}

	void UIManager::Pop(enums::UIType type)
	{
		if (m_UIBases.empty())
		{
			return;
		}

		std::stack<UIBase*> tempStack;

		while (!m_UIBases.empty())
		{
			UIBase* uiBase = m_UIBases.top();
			m_UIBases.pop();

			if (!uiBase)
			{
				continue;
			}

			// 제거할 종류가 아니면 임시 보관
			if (uiBase->GetType() != type)
			{
				tempStack.push(uiBase);
				continue;
			}

			// 전체 화면 UI를 닫으면 아래 UI 하나를 다시 활성화
			if (uiBase->IsFullScreen())
			{
				std::stack<UIBase*> uiBases = m_UIBases;

				while (!uiBases.empty())
				{
					UIBase* previousUI = uiBases.top();
					uiBases.pop();

					if (previousUI)
					{
						previousUI->Active();
						break;
					}
				}
			}

			uiBase->UIClear();
		}

		// 제거하지 않은 UI를 원래 순서로 복구
		while (!tempStack.empty())
		{
			UIBase* uiBase = tempStack.top();
			tempStack.pop();

			m_UIBases.push(uiBase);
		}
	}

	void UIManager::Release()
	{
		for (auto& iter : m_UIs)
		{
			delete iter.second;
			iter.second = nullptr;
		}

		m_UIs.clear();

		while (!m_UIBases.empty())
		{
			m_UIBases.pop();
		}

		while (!m_requestUIQueue.empty())
		{
			m_requestUIQueue.pop();
		}

		m_activeUI = nullptr;
	}
}