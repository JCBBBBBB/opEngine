#include "opAnimator.h"

namespace op
{
	Animator::Animator() : Component(enums::ComponentType::Animator), m_animations{}, m_activeAnimation(nullptr), m_isLoop(false)
	{

	}

	Animator::~Animator()
	{
		for (auto& iter : m_animations)
		{
			delete iter.second;
			iter.second = nullptr;
		}

		for (auto& iter : m_events)
		{
			delete iter.second;
			iter.second = nullptr;
		}
	}

	void Animator::Initialize()
	{

	}

	void Animator::Update()
	{
		if (m_activeAnimation)
		{
			m_activeAnimation->Update(); // 현재 Animation의 시간을 갱신하고 프레임 변경 여부 판단

			Events* events = FindEvents(m_activeAnimation->GetName());

			if (m_activeAnimation->IsComplete() == true) // 마지막 프레임까지 끝났고 반복 설정이 켜져 있으면
			{
				if (events)
				{
					events->completeEvent();

					if (m_isLoop == true)
					{
						m_activeAnimation->Reset();
					}
				}

				m_activeAnimation->Reset(); // 첫 프레임으로 되돌림
			}

		}
	}

	void Animator::LateUpdate()
	{

	}

	void Animator::Render(HDC hdc)
	{
		if (m_activeAnimation)
		{
			m_activeAnimation->Render(hdc);
		}
	}

	// Animation 하나를 만들어 등록
	void Animator::CreateAnimation(const std::wstring& name, graphics::Texture* spriteSheet, Vector2 leftTop, Vector2 size, Vector2 offset, UINT spriteLength, float duration)
	{
		Animation* animation = nullptr;
		animation = FindAnimation(name); // name 이름을 가진 애니메이션이 이미 등록이 되어있는지 확인

		if (animation != nullptr) // 이미 있으면 함수 종료
		{
			return;
		}

		animation = new Animation(); // 새로운 Animation 객체 생성
		animation->SetName(name);
		animation->CreateAnimation(name, spriteSheet, leftTop, size, offset, spriteLength, duration); // Animation 내부에 원본 이미지와 프레임 정보들을 구성
		animation->SetAnimator(this); // 이 Animation을 관리하는 Animator가 자신이라는 것을 저장

		Events* events = new Events();
		m_events.insert(std::make_pair(name, events));

		m_animations.insert(std::make_pair(name, animation)); // CatFrontMove라는 이름과 Animation 주소를 목록에 등록
	}

	Animation* Animator::FindAnimation(const std::wstring& name)
	{
		auto iter = m_animations.find(name); // 이름 이용해서 map에서 항목 찾기

		if (iter == m_animations.end())
		{
			return nullptr;
		}

		return iter->second; // first는  이름, second는 Animation 주소
	}

	// Update와 Render가 실행될 때 재생되도록 준비하는 함수
	void Animator::PlayAnimation(const std::wstring& name, bool loop)
	{
		Animation* animation = FindAnimation(name); // 목록에서 name 이름을 가진 Animation을 찾음

		if (animation == nullptr)
		{
			return; // 해당 이름이 없으면 종료
		}

		if (m_activeAnimation)
		{
			Events* currentEvents = FindEvents(m_activeAnimation->GetName());

			if (currentEvents)
			{
				currentEvents->endEvent();
			}
		}

		Events* nextEvents = FindEvents(animation->GetName());
		if (nextEvents)
		{
			nextEvents->startEvent();
		}

		m_activeAnimation = animation; // 매번 Update와 Render에서 사용할 Animation 주소 저장
		m_activeAnimation->Reset(); // 첫 프레임부터 시작하도록 재생 상태 초기화
		m_isLoop = loop; // true이므로 끝나면 반복 재생
	}

	Animator::Events* Animator::FindEvents(const std::wstring& name)
	{
		auto iter = m_events.find(name);

		if (iter == m_events.end())
		{
			return nullptr;
		}

		return iter->second;
	}

	std::function<void()>& Animator::GetStartEvent(const std::wstring& name)
	{
		Events* events = FindEvents(name);
		return events->startEvent.m_event;
	}

	std::function<void()>& Animator::GetCompleteEvent(const std::wstring& name)
	{
		Events* events = FindEvents(name);
		return events->completeEvent.m_event;
	}

	std::function<void()>& Animator::GetEndEvent(const std::wstring& name)
	{
		Events* events = FindEvents(name);
		return events->endEvent.m_event;
	}
}