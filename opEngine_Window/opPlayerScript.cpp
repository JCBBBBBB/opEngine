#include "opPlayerScript.h"
#include "opInput.h"
#include "opTransform.h"
#include "opTime.h"
#include "opGameObject.h"

namespace op
{
	PlayerScript::PlayerScript()
	{
	}

	PlayerScript::~PlayerScript()
	{
	}

	void PlayerScript::Initialize()
	{
	}

	void PlayerScript::Update()
	{
		if (Input::GetKey(KeyCode::Right))
		{
			Transform* transform = GetOwner()->GetComponent<Transform>();
			Vector2 pos = transform->GetPosition();
			pos.x += 100.0f * Time::DeltaTime();
			transform->SetPosition(pos);
		}
		if (Input::GetKey(KeyCode::Left))
		{
			Transform* transform = GetOwner()->GetComponent<Transform>();
			Vector2 pos = transform->GetPosition();
			pos.x -= 100.0f * Time::DeltaTime();
			transform->SetPosition(pos);
		}
		if (Input::GetKey(KeyCode::Up))
		{
			Transform* transform = GetOwner()->GetComponent<Transform>();
			Vector2 pos = transform->GetPosition();
			pos.y -= 100.0f * Time::DeltaTime();
			transform->SetPosition(pos);
		}
		if (Input::GetKey(KeyCode::Down))
		{
			Transform* transform = GetOwner()->GetComponent<Transform>();
			Vector2 pos = transform->GetPosition();
			pos.y += 100.0f * Time::DeltaTime();
			transform->SetPosition(pos);
		}
	}

	void PlayerScript::LateUpdate()
	{
	}

	void PlayerScript::Render(HDC hdc)
	{
	}
}