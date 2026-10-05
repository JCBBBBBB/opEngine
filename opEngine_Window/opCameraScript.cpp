#include "opCameraScript.h"

#include "..\\opEngine_SOURCE\\opInput.h"
#include "..\\opEngine_SOURCE\\opTransform.h"
#include "..\\opEngine_SOURCE\\opTime.h"
#include "..\\opEngine_SOURCE\\opGameObject.h"
#include "..\\opEngine_SOURCE\\opApplication.h"

extern op::Application application;

namespace op
{
	CameraScript::CameraScript()
	{
	}

	CameraScript::~CameraScript()
	{
	}

	void CameraScript::Initialize()
	{
	}

	void CameraScript::Update()
	{
		// 타일용 창이나 다른 프로그램을 사용할 때는 이동하지 않는다.
		if (GetForegroundWindow() != application.GetHwnd())
		{
			return;
		}

		Transform* transform = GetOwner()->GetComponent<Transform>();
		Vector2 pos = transform->GetPosition();

		if (Input::GetKey(KeyCode::Right))
		{
			pos.x += 100.0f * Time::DeltaTime();
		}

		if (Input::GetKey(KeyCode::Left))
		{
			pos.x -= 100.0f * Time::DeltaTime();
		}

		if (Input::GetKey(KeyCode::Up))
		{
			pos.y -= 100.0f * Time::DeltaTime();
		}

		if (Input::GetKey(KeyCode::Down))
		{
			pos.y += 100.0f * Time::DeltaTime();
		}

		transform->SetPosition(pos);
	}

	void CameraScript::LateUpdate()
	{
	}

	void CameraScript::Render(HDC hdc)
	{
	}
}