#include "opUIBase.h"

namespace op
{
	UIBase::UIBase(enums::UIType type)
		: m_position(math::Vector2::Zero)
		, m_size(math::Vector2::Zero)
		, m_mouseOn(false)
		, m_type(type)
		, m_fullScreen(false)
		, m_enabled(false)
		, m_parent(nullptr)
	{
	}

	UIBase::~UIBase()
	{
	}

	void UIBase::Initialize()
	{
		OnInit();
	}

	void UIBase::Active()
	{
		m_enabled = true;
		OnActive();
	}

	void UIBase::InActive()
	{
		m_enabled = false;
		OnInActive();
	}

	void UIBase::Update()
	{
		if (m_enabled)
		{
			OnUpdate();
		}
	}

	void UIBase::LateUpdate()
	{
		if (m_enabled)
		{
			OnLateUpdate();
		}
	}

	void UIBase::Render(HDC hdc)
	{
		if (m_enabled)
		{
			OnRender(hdc);
		}
	}

	void UIBase::UIClear()
	{
		OnClear();
	}

	void UIBase::OnInit()
	{
	}

	void UIBase::OnActive()
	{
	}

	void UIBase::OnInActive()
	{
	}

	void UIBase::OnUpdate()
	{
	}

	void UIBase::OnLateUpdate()
	{
	}

	void UIBase::OnRender(HDC hdc)
	{
	}

	void UIBase::OnClear()
	{
	}
}