#include "opCatScript.h"
#include "opInput.h"
#include "opTransform.h"
#include "opTime.h"
#include "opGameObject.h"
#include "opAnimator.h"
#include "opObject.h"

namespace op
{
	CatScript::CatScript() :m_dest(Vector2::Zero)
		, m_state(State::SitDown)
		, m_animator(nullptr)
		, m_direction(Direction::Down)
		, m_time(0.0f)
		, m_deathTime(0.0f)
		, m_player(nullptr)
		, m_radian(0.0f)
	{
	}

	CatScript::~CatScript()
	{
	}

	void CatScript::Initialize()
	{
	}

	void CatScript::Update()
	{
		m_deathTime += Time::DeltaTime();

		if (m_deathTime > 6.0f)
		{
			//object::Destroy(GetOwner());
		}

		if (m_animator == nullptr)
		{
			m_animator = GetOwner()->GetComponent<Animator>();
		}

		switch (m_state)
		{
		case op::CatScript::State::SitDown:
			SitDown();
			break;

		case op::CatScript::State::Walk:
			Move();
			break;

		case op::CatScript::State::Sleep:
			break;

		case op::CatScript::State::LayDown:
			break;

		case op::CatScript::State::Attack:
			break;

		default:
			break;
		}
	}

	void CatScript::LateUpdate()
	{
	}

	void CatScript::Render(HDC hdc)
	{
	}

	void CatScript::SitDown()
	{
        // 원본 BoxCollider 단계에서는 대기 중 이동 계산을 사용하지 않는다.
        m_time += Time::DeltaTime();
	}

	void CatScript::Move()
	{
		m_time += Time::DeltaTime();
		if (m_time > 2.0f)
		{
			int isLayDown = rand() % 2;
			if (isLayDown)
			{
				m_state = State::LayDown;
				m_animator->PlayAnimation(L"LayDown", false);
			}
			else
			{
				m_state = State::SitDown;
				m_animator->PlayAnimation(L"SitDown", false);
			}
		}

		Transform* transform = GetOwner()->GetComponent<Transform>();
		Translate(transform);
	}

	void CatScript::LayDown()
	{

	}

	void CatScript::PlayWalkAnimationByDirection(Direction dir)
	{
		switch (dir)
		{
		case op::CatScript::Direction::Left:
			m_animator->PlayAnimation(L"LeftWalk", true);
			break;

		case op::CatScript::Direction::Right:
			m_animator->PlayAnimation(L"RightWalk", true);
			break;

		case op::CatScript::Direction::Up:
			m_animator->PlayAnimation(L"UpWalk", true);
			break;

		case op::CatScript::Direction::Down:
			m_animator->PlayAnimation(L"DownWalk", true);
			break;

		default:
			assert(false);
			break;
		}

	}

	void CatScript::Translate(Transform* tr)
	{
		Vector2 pos = tr->GetPosition();
		switch (m_direction)
		{
		case op::CatScript::Direction::Left:
			pos.x -= 100.0f * Time::DeltaTime();
			break;

		case op::CatScript::Direction::Right:
			pos.x += 100.0f * Time::DeltaTime();
			break;

		case op::CatScript::Direction::Up:
			pos.y -= 100.0f * Time::DeltaTime();
			break;

		case op::CatScript::Direction::Down:
			pos.y += 100.0f * Time::DeltaTime();
			break;

		default:
			assert(false);
			break;
		}
		tr->SetPosition(pos);
	}
}