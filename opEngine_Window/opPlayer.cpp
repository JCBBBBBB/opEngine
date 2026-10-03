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

	}

	void Player::Render(HDC hdc)
	{
		GameObject::Render(hdc);
	}
}