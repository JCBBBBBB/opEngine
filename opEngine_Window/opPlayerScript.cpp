#include "..\\opEngine_SOURCE\\opRigidbody.h"

#include "opRigidbody.h"
#include "opUIManager.h"
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
	PlayerScript::PlayerScript() : m_state(PlayerScript::State::Idle), m_animator(nullptr), m_pixelMap(nullptr)
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
		GameObject* owner = GetOwner();
		if (owner == nullptr)
		{
			return;
		}

		if (m_animator == nullptr)
		{
			m_animator = owner->GetComponent<Animator>();
		}

		// 기존 플레이어 행동 처리
		switch (m_state)
		{
		case State::Idle:
			Idle();
			break;

		case State::Walk:
			Move();
			break;

		case State::Sleep:
			break;

		case State::GiveWater:
			GiveWater();
			break;

		case State::Attack:
			break;

		default:
			break;
		}

		// 여기부터 이번 커밋에서 추가하는 픽셀 충돌 처리
		if (m_pixelMap == nullptr)
		{
			return;
		}

		Transform* transform = owner->GetComponent<Transform>();
		Rigidbody* playerRb = owner->GetComponent<Rigidbody>();

		if (transform == nullptr || playerRb == nullptr)
		{
			return;
		}

		Vector2 pos = transform->GetPosition();

		// 플레이어 중심보다 아래쪽 50픽셀, 즉 발밑의 색을 읽는다.
		COLORREF color = m_pixelMap->GetPixel(
			static_cast<int>(pos.x),
			static_cast<int>(pos.y + 50.0f)
		);

		if (color == RGB(255, 0, 0))
		{
			// 발밑이 빨간색이면 바닥에 닿았다고 판단한다.
			playerRb->SetGround(true);

			// 바닥에 들어간 플레이어를 위로 1픽셀 이동시킨다.
			pos.y -= 1.0f;
			transform->SetPosition(pos);
		}
		else
		{
			// 빨간색이 아니면 공중에 있다고 판단한다.
			playerRb->SetGround(false);
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

		if (Input::GetKeyDown(KeyCode::W) && rigidbody->IsGround())
		{
			Vector2 velocity = rigidbody->GetVelocity();

			// 음수 y 속도는 위쪽 이동
			velocity.y = -500.0f;

			rigidbody->SetVelocity(velocity);
			rigidbody->SetGround(false);
		}

		if (Input::GetKeyUp(KeyCode::D)
			|| Input::GetKeyUp(KeyCode::A)
			|| Input::GetKeyUp(KeyCode::W))
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

		if (Input::GetKey(KeyCode::A)
			|| Input::GetKey(KeyCode::D)
			|| Input::GetKeyDown(KeyCode::W))
		{
			m_state = State::Walk;
			Move();
		}

		if (Input::GetKeyDown(KeyCode::I))
		{
			UIManager::Push(enums::UIType::HpBar);
		}

		if (Input::GetKeyDown(KeyCode::O))
		{
			UIManager::Pop(enums::UIType::HpBar);
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
    }
    void PlayerScript::OnCollisionStay(Collider*) {}
    void PlayerScript::OnCollisionExit(Collider*) {}
}