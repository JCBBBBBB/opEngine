#include "opPlayerScript.h"
#include "opInput.h"
#include "opTransform.h"
#include "opTime.h"
#include "opGameObject.h"
#include "opAnimator.h"


namespace op
{
	PlayerScript::PlayerScript() : m_state(PlayerScript::State::Idle), m_animator(nullptr)
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
		if (m_animator == nullptr)
		{
			m_animator = GetOwner()->GetComponent<Animator>();
		}

		switch (m_state)
		{
		case op::PlayerScript::State::Idle:
			Idle();
			break;

		case op::PlayerScript::State::Walk:
			Move();
			break;

		case op::PlayerScript::State::Sleep:
			break;

		case op::PlayerScript::State::GiveWater:
			GiveWater();
			break;

		case op::PlayerScript::State::Attack:
			break;

		default:
			break;
		}
	}

	void PlayerScript::LateUpdate()
	{
	}

	void PlayerScript::Render(HDC hdc)
	{

	}

	
	void PlayerScript::Move()
	{
		Transform* transform = GetOwner()->GetComponent<Transform>();
		Vector2 position = transform->GetPosition();

		if (Input::GetKey(KeyCode::D))
		{
			position.x += 100.0f * Time::DeltaTime();
		}
		if (Input::GetKey(KeyCode::A))
		{
			position.x -= 100.0f * Time::DeltaTime();
		}
		if (Input::GetKey(KeyCode::W))
		{
			position.y -= 100.0f * Time::DeltaTime();
		}
		if (Input::GetKey(KeyCode::S))
		{
			position.y += 100.0f * Time::DeltaTime();
		}

		transform->SetPosition(position);

		if (Input::GetKeyUp(KeyCode::D) || Input::GetKeyUp(KeyCode::A) || Input::GetKeyUp(KeyCode::W) || Input::GetKeyUp(KeyCode::S))
		{
			m_state = PlayerScript::State::Idle;
			m_animator->PlayAnimation(L"Idle", false);
		}
	}
	void PlayerScript::Idle()
	{
		if (Input::GetKey(KeyCode::LButton))
		{
			m_state = PlayerScript::State::GiveWater;
			m_animator->PlayAnimation(L"FrontGiveWater", false);

			return;
		}


		if (Input::GetKey(KeyCode::W) || Input::GetKey(KeyCode::A) || Input::GetKey(KeyCode::S) || Input::GetKey(KeyCode::D))
		{
			m_state = State::Walk;
		}
	}


	void PlayerScript::GiveWater()
	{
		if (m_animator->IsComplete())
		{
			m_state = State::Idle;
			m_animator->PlayAnimation(L"Idle", false);
		}
	}
}