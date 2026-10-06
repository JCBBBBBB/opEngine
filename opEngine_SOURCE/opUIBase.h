#pragma once
#include "opEntity.h"

namespace op
{
	class UIBase : public Entity
	{
	public:
		// 실행할 함수를 저장하는 구조체
		struct Event
		{
			void operator=(std::function<void()> func)
			{
				m_event = std::move(func);
			}

			void operator()()
			{
				if (m_event)
				{
					m_event();
				}
			}

			std::function<void()> m_event;
		};

		UIBase(enums::UIType type);
		virtual ~UIBase();

		void Initialize();
		void Active();
		void InActive();

		void Update();
		void LateUpdate();
		void Render(HDC hdc);

		void UIClear();

		virtual void OnInit();
		virtual void OnActive();
		virtual void OnInActive();
		virtual void OnUpdate();
		virtual void OnLateUpdate();
		virtual void OnRender(HDC hdc);
		virtual void OnClear();

		enums::UIType GetType() { return m_type; }
		void SetType(enums::UIType type) { m_type = type; }

		void SetFullScreen(bool enable) { m_fullScreen = enable; }
		bool IsFullScreen() { return m_fullScreen; }

		math::Vector2 GetPos() { return m_position; }
		void SetPos(math::Vector2 position) { m_position = position; }

		math::Vector2 GetSize() { return m_size; }
		void SetSize(math::Vector2 size) { m_size = size; }

	protected:
		math::Vector2 m_position; // 화면에서 UI의 왼쪽 위 위치
		math::Vector2 m_size;     // UI의 너비와 높이
		bool m_mouseOn;          // 마우스가 UI 영역 안에 있는지

	private:
		enums::UIType m_type;
		bool m_fullScreen;
		bool m_enabled;

		UIBase* m_parent; // 이번 단계에서는 아직 사용하지 않음
	};
}