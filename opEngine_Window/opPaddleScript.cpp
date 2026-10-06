#include "opPaddleScript.h"

#include "../opEngine_SOURCE/opApplication.h"
#include "../opEngine_SOURCE/opGameObject.h"
#include "../opEngine_SOURCE/opInput.h"
#include "../opEngine_SOURCE/opTime.h"
#include "../opEngine_SOURCE/opTransform.h"
#include "../opEngine_SOURCE/opCamera.h"
#include "../opEngine_SOURCE/opRenderer.h"

extern op::Application application;

namespace op
{
	PaddleScript::PaddleScript() : m_speed(400.0f), m_size(120.0f, 20.0f)
	{
	}
	PaddleScript::~PaddleScript()
	{
	}
	void PaddleScript::Update()
	{
		// 게임 창이 선택되어 있을 때만 조작한다.
		if (GetForegroundWindow() != application.GetHwnd())
		{
			return;
		}

		GameObject* owner = GetOwner();
		if (owner == nullptr)
		{
			return;
		}

		Transform* transform = owner->GetComponent<Transform>();
		if (transform == nullptr)
		{
			return;
		}

		Vector2 pos = transform->GetPosition();

		bool left = Input::GetKeyDown(KeyCode::A) || Input::GetKey(KeyCode::A) || Input::GetKeyDown(KeyCode::Left) || Input::GetKey(KeyCode::Left);
		bool right = Input::GetKeyDown(KeyCode::D) || Input::GetKey(KeyCode::D) || Input::GetKeyDown(KeyCode::Right) || Input::GetKey(KeyCode::Right);

		float direction = 0.0f;

		if (left)
		{
			direction -= 1.0f;
		}

		if (right)
		{
			direction += 1.0f;
		}

		pos.x += direction * m_speed * Time::DeltaTime();
		
		float maxX = static_cast<float>(application.GetWidth()) - m_size.x;

		if (maxX < 0.0f)
		{
			maxX = 0.0f;
		}

		if (pos.x < 0.0f)
		{
			pos.x = 0.0f;
		}

		else if (pos.x > maxX)
		{
			pos.x = maxX;
		}

		transform->SetPosition(pos);
	}

	void PaddleScript::Render(HDC hdc)
	{
		GameObject* owner = GetOwner();
		if (owner == nullptr || hdc == nullptr)
		{
			return;
		}

		Transform* transform = owner->GetComponent<Transform>();
		if (transform == nullptr)
		{
			return;
		}

		Vector2 pos = transform->GetPosition();

		if (renderer::mainCamera)
		{
			pos = renderer::mainCamera->CalculatePosition(pos);
		}

		// 이번 단계에서는 위치를 막대의 왼쪽 위로 사용한다.
		RECT rect = {};
		rect.left = static_cast<LONG>(pos.x);
		rect.top = static_cast<LONG>(pos.y);
		rect.right = static_cast<LONG>(pos.x + m_size.x);
		rect.bottom = static_cast<LONG>(pos.y + m_size.y);

		HBRUSH brush = CreateSolidBrush(RGB(255, 255, 255));

		FillRect(hdc, &rect, brush);

		DeleteObject(brush);
	}
}