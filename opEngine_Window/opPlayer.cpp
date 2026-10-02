#include "opPlayer.h"
#include "opInput.h"
#include "opTransform.h"
#include "opTime.h"


namespace op
{
	void Player::Initialize()
	{
		GameObject::Initialize();
	}

	void Player::Update()
	{
		GameObject::Update();
	}

	void Player::LateUpdate()
	{
		GameObject::LateUpdate();

		if (Input::GetKey(KeyCode::Right))
		{
			Transform* transform = GetComponent<Transform>();
			Vector2 pos = transform->GetPosition();
			pos.x += 100.0f * Time::DeltaTime();
			transform->SetPos(pos);
		}
	}

	void Player::Render(HDC hdc)
	{
		GameObject::Render(hdc);
	}
}