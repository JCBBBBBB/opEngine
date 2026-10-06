#include "opUIButton.h"
#include "opInput.h"

namespace op
{
	UIButton::UIButton()
		: UIBase(enums::UIType::Button)
		, m_texture(nullptr)
	{
	}

	UIButton::~UIButton()
	{
	}

	void UIButton::OnInit()
	{
		SetPos(math::Vector2(200.0f, 200.0f));
		SetSize(math::Vector2(200.0f, 200.0f));

		// 클릭 이벤트에 자신의 ButtonClick 함수 연결
		m_onClick = std::bind(&UIButton::ButtonClick, this);
	}

	void UIButton::OnActive()
	{
	}

	void UIButton::OnInActive()
	{
	}

	void UIButton::OnUpdate()
	{
		math::Vector2 mousePos = Input::GetMousePosition();

		// 마우스가 버튼 사각형 안에 있는지 확인
		if (m_position.x <= mousePos.x
			&& mousePos.x <= m_position.x + m_size.x
			&& m_position.y <= mousePos.y
			&& mousePos.y <= m_position.y + m_size.y)
		{
			m_mouseOn = true;
		}
		else
		{
			m_mouseOn = false;
		}

		// 버튼 위에서 마우스 왼쪽 버튼을 누른 순간
		if (Input::GetKeyDown(KeyCode::LButton))
		{
			if (m_mouseOn)
			{
				m_onClick();
			}
		}
	}

	void UIButton::OnLateUpdate()
	{
	}

	void UIButton::OnRender(HDC hdc)
	{
		Rectangle(
			hdc,
			static_cast<int>(m_position.x),
			static_cast<int>(m_position.y),
			static_cast<int>(m_position.x + m_size.x),
			static_cast<int>(m_position.y + m_size.y));
	}

	void UIButton::OnClear()
	{
	}

	void UIButton::ButtonClick()
	{
		// 원본은 실제 기능 없이 이 줄만 존재한다.
		// 여기에 중단점을 걸면 클릭 여부를 확인할 수 있다.
		int a = 0;
	}
}