#include "opAnimator.h"

namespace op
{
	Animator::Animator() : Component(enums::ComponentType::Animator), m_animations{}, m_activeAnimation(nullptr), m_isLoop(false)
	{

	}

	Animator::~Animator()
	{

	}

	void Animator::Initialize()
	{

	}

	void Animator::Update()
	{
		if (m_activeAnimation)
		{
			m_activeAnimation->Update();

			if (m_activeAnimation->IsComplete() == true && m_isLoop == true)
			{
				m_activeAnimation->Reset();
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

	// 애니메이션 생성해서 m_animations에 등록
	void Animator::CreateAnimation(const std::wstring& name, graphics::Texture* spriteSheet, Vector2 leftTop, Vector2 size, Vector2 offset, UINT spriteLength, float duration)
	{
		Animation* animation = nullptr;
		animation = FindAnimation(name);

		if (animation != nullptr)
		{
			return;
		}

		animation = new Animation();
		animation->CreateAnimation(name, spriteSheet, leftTop, size, offset, spriteLength, duration);
		animation->SetAnimator(this);

		m_animations.insert(std::make_pair(name, animation));
	}

	Animation* Animator::FindAnimation(const std::wstring& name)
	{
		auto iter = m_animations.find(name);
		if (iter == m_animations.end())
		{
			return nullptr;
		}

		return iter->second;
	}

	void Animator::PlayAnimation(const std::wstring& name, bool loop)
	{
		Animation* animation = FindAnimation(name);
		if (animation == nullptr)
		{
			return;
		}

		m_activeAnimation = animation;
		m_activeAnimation->Reset();
		m_isLoop = loop;
	}

}