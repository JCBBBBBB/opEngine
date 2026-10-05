#include "..\\opEngine_SOURCE\\opRigidbody.h"

#include "opPlayerScript.h"
#include "opCollider.h"
#include "opInput.h"
#include "opTransform.h"
#include "opTime.h"
#include "opGameObject.h"
#include "opAnimator.h"

#include "opCat.h"
#include "opCatScript.h"
#include "opObject.h"
#include "opResources.h"

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
		Rigidbody* rigidbody = GetOwner()->GetComponent<Rigidbody>();

		if (!rigidbody)
		{
			return;
		}

		if (Input::GetKey(KeyCode::D))
		{
			rigidbody->AddForce(Vector2(200.0f, 0.0f));
		}

		if (Input::GetKey(KeyCode::A))
		{
			rigidbody->AddForce(Vector2(-200.0f, 0.0f));
		}

		// 화면 좌표는 y가 작아질수록 위쪽이다.
		if (Input::GetKey(KeyCode::W))
		{
			rigidbody->AddForce(Vector2(0.0f, -200.0f));
		}

		if (Input::GetKey(KeyCode::S))
		{
			rigidbody->AddForce(Vector2(0.0f, 200.0f));
		}

		if (Input::GetKeyUp(KeyCode::D)
			|| Input::GetKeyUp(KeyCode::A)
			|| Input::GetKeyUp(KeyCode::W)
			|| Input::GetKeyUp(KeyCode::S))
		{
			m_state = State::Idle;

			if (m_animator)
			{
				m_animator->PlayAnimation(L"Idle", false);
			}
		}
	}
	void PlayerScript::Idle()
	{
		if (Input::GetKey(KeyCode::LButton))
		{
			Cat* cat = object::Instantiate<Cat>(
				enums::LayerType::Animal);

			CatScript* catScript = cat->AddComponent<CatScript>();
			catScript->SetPlayer(GetOwner());

			graphics::Texture* catTexture =
				Resources::Find<graphics::Texture>(L"Cat");

			Animator* catAnimator = cat->AddComponent<Animator>();

			catAnimator->CreateAnimation(
				L"DownWalk", catTexture,
				Vector2(0.0f, 0.0f), Vector2(32.0f, 32.0f),
				Vector2::Zero, 4, 0.1f);

			catAnimator->CreateAnimation(
				L"RightWalk", catTexture,
				Vector2(0.0f, 32.0f), Vector2(32.0f, 32.0f),
				Vector2::Zero, 4, 0.1f);

			catAnimator->CreateAnimation(
				L"UpWalk", catTexture,
				Vector2(0.0f, 64.0f), Vector2(32.0f, 32.0f),
				Vector2::Zero, 4, 0.1f);

			catAnimator->CreateAnimation(
				L"LeftWalk", catTexture,
				Vector2(0.0f, 96.0f), Vector2(32.0f, 32.0f),
				Vector2::Zero, 4, 0.1f);

			catAnimator->CreateAnimation(
				L"SitDown", catTexture,
				Vector2(0.0f, 128.0f), Vector2(32.0f, 32.0f),
				Vector2::Zero, 4, 0.1f);

			catAnimator->CreateAnimation(
				L"Grooming", catTexture,
				Vector2(0.0f, 160.0f), Vector2(32.0f, 32.0f),
				Vector2::Zero, 4, 0.1f);

			catAnimator->CreateAnimation(
				L"LayDown", catTexture,
				Vector2(0.0f, 192.0f), Vector2(32.0f, 32.0f),
				Vector2::Zero, 4, 0.1f);

			catAnimator->PlayAnimation(L"SitDown", false);

			Transform* playerTransform =
				GetOwner()->GetComponent<Transform>();

			cat->GetComponent<Transform>()->SetPosition(
				playerTransform->GetPosition());

			cat->GetComponent<Transform>()->SetScale(
				Vector2(2.0f, 2.0f));

			Vector2 mousePos = Input::GetMousePosition();
			catScript->m_dest = mousePos;
		}

		if (Input::GetKey(KeyCode::W)
			|| Input::GetKey(KeyCode::A)
			|| Input::GetKey(KeyCode::S)
			|| Input::GetKey(KeyCode::D))
		{
			m_state = State::Walk;

			// 걷기 상태로 바뀐 프레임에도 힘을 전달한다.
			Move();
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

	void PlayerScript::AttackEffect()
	{
		Cat* cat = object::Instantiate<Cat>(
			enums::LayerType::Animal);

		CatScript* catScript = cat->AddComponent<CatScript>();
		catScript->SetPlayer(GetOwner());

		graphics::Texture* catTexture =
			Resources::Find<graphics::Texture>(L"Cat");

		Animator* catAnimator = cat->AddComponent<Animator>();

		catAnimator->CreateAnimation(
			L"DownWalk", catTexture,
			Vector2(0.0f, 0.0f), Vector2(32.0f, 32.0f),
			Vector2::Zero, 4, 0.1f);

		catAnimator->CreateAnimation(
			L"RightWalk", catTexture,
			Vector2(0.0f, 32.0f), Vector2(32.0f, 32.0f),
			Vector2::Zero, 4, 0.1f);

		catAnimator->CreateAnimation(
			L"UpWalk", catTexture,
			Vector2(0.0f, 64.0f), Vector2(32.0f, 32.0f),
			Vector2::Zero, 4, 0.1f);

		catAnimator->CreateAnimation(
			L"LeftWalk", catTexture,
			Vector2(0.0f, 96.0f), Vector2(32.0f, 32.0f),
			Vector2::Zero, 4, 0.1f);

		catAnimator->CreateAnimation(
			L"SitDown", catTexture,
			Vector2(0.0f, 128.0f), Vector2(32.0f, 32.0f),
			Vector2::Zero, 4, 0.1f);

		catAnimator->CreateAnimation(
			L"Grooming", catTexture,
			Vector2(0.0f, 160.0f), Vector2(32.0f, 32.0f),
			Vector2::Zero, 4, 0.1f);

		catAnimator->CreateAnimation(
			L"LayDown", catTexture,
			Vector2(0.0f, 192.0f), Vector2(32.0f, 32.0f),
			Vector2::Zero, 4, 0.1f);

		catAnimator->PlayAnimation(L"SitDown", false);

		Transform* playerTransform =
			GetOwner()->GetComponent<Transform>();

		cat->GetComponent<Transform>()->SetPosition(
			playerTransform->GetPosition());

		cat->GetComponent<Transform>()->SetScale(
			Vector2(2.0f, 2.0f));

		Vector2 mousePos = Input::GetMousePosition();
		catScript->m_dest = mousePos;
	}
    void PlayerScript::OnCollisionEnter(Collider* other)
    {
        // 원본 데모: 플레이어와 충돌한 상대를 (400, 500)으로 이동시킨다.
        if (other && other->GetOwner())
            if (auto* transform = other->GetOwner()->GetComponent<Transform>())
                transform->SetPosition(Vector2(400.0f, 500.0f));
    }
    void PlayerScript::OnCollisionStay(Collider*) {}
    void PlayerScript::OnCollisionExit(Collider*) {}
}