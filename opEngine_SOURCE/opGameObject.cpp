#include "opGameObject.h"
#include "opInput.h"
#include "opTime.h"

namespace op
{

	GameObject::GameObject() : m_x(0), m_y(0)
	{

	}

	GameObject::~GameObject()
	{

	}
	
	void GameObject::Update()
	{
		const int speed = 100.0f;

		if (Input::GetKey(KeyCode::A))
		{
			m_x -= speed * Time::DeltaTime();
		}

		if (Input::GetKey(KeyCode::D))
		{
			m_x += speed * Time::DeltaTime();
		}

		if (Input::GetKey(KeyCode::W))
		{
			m_y -= speed * Time::DeltaTime();
		}

		if (Input::GetKey(KeyCode::S))
		{
			m_y += speed * Time::DeltaTime();
		}
	}

	void GameObject::LateUpdate()
	{
	
	}

	void GameObject::Render(HDC hdc)
	{
		//파랑 브러쉬 생성
		HBRUSH blueBrush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));

		// 파랑 브러쉬 DC에 선택 그리고 흰색 브러쉬 반환값 반환
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, blueBrush);

		HPEN redPen = CreatePen(PS_SOLID, 2, RGB(rand() % 255, rand() % 255, rand() % 255));
		HPEN oldPen = (HPEN)SelectObject(hdc, redPen);

		SelectObject(hdc, oldPen);

		Ellipse(hdc, m_x, m_y, 100 + m_x, 100 + m_y);

		SelectObject(hdc, oldBrush);
		DeleteObject(blueBrush);
		DeleteObject(redPen);
	}
	
}