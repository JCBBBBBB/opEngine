#pragma once
#include "opComponent.h"
#include "opAnimation.h"

namespace op
{	
	// 애니메이션 목록과 현재 애니메이션 관리
	//Animator -> 어느 Animation을 재생할지 선택하고 실행
	class Animator : public Component
	{
	public:
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

		struct Events
		{
			Event startEvent; // 애니메이션이 시작할 때 실행할 함수
			Event completeEvent; // 끝까지 재생했을 때 실행할 함수
			Event endEvent; // 해당 애니메이션의 재생을 종료할 때 실행할 함수
		};



		Animator();
		~Animator();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;

		void CreateAnimation(const std::wstring& name, graphics::Texture* spriteSheet, Vector2 leftTop, Vector2 size, Vector2 offset, UINT spriteLength, float duration);

		Animation* FindAnimation(const std::wstring& name);
		void PlayAnimation(const std::wstring& name, bool loop = true);

		Events* FindEvents(const std::wstring& name);
		std::function<void()>& GetStartEvent(const std::wstring& name);
		std::function<void()>& GetCompleteEvent(const std::wstring& name);
		std::function<void()>& GetEndEvent(const std::wstring& name);


		bool IsComplete()
		{
			return m_activeAnimation->IsComplete();
		}

	private:
		std::map<std::wstring, Events*> m_events;
		std::map<std::wstring, Animation*> m_animations; // 이름으로 찾을 수 있는 애니메이션 목록
		Animation* m_activeAnimation; // 현재 Update와 Render를 실행할 Animation의 주소
		bool m_isLoop; // 현재 Animation이 끝나면 첫 프레임부터 반복할지 여부
	};
}
