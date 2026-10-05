#include "opRigidbody.h"
#include "opTime.h"
#include "opTransform.h"
#include "opGameObject.h"

namespace op
{
	Rigidbody::Rigidbody()
		: Component(enums::ComponentType::Rigidbody)
		, m_mass(1.0f)
		, m_friction(10.0f)
		, m_force(Vector2::Zero)
		, m_acceleration(Vector2::Zero)
		, m_velocity(Vector2::Zero)
		, m_gravity(Vector2::Zero)
	{
	}

	Rigidbody::~Rigidbody()
	{
	}

	void Rigidbody::Initialize()
	{
	}

	void Rigidbody::Update()
	{
		if (!GetOwner())
		{
			m_force.clear();
			return;
		}

		Transform* transform = GetOwner()->GetComponent<Transform>();

		if (!transform)
		{
			m_force.clear();
			return;
		}

		const float deltaTime = Time::DeltaTime();

		// F = m × a → a = F / m
		m_acceleration = m_force / m_mass;

		// 가속도를 이용해 속도 변경
		m_velocity += m_acceleration * deltaTime;

		if (!(m_velocity == Vector2::Zero))
		{
			// 현재 속도의 반대 방향
			Vector2 friction = -m_velocity;

			// 방향을 길이 1로 만든 후 감속량 계산
			friction =
				friction.normalize()
				* m_friction
				* m_mass
				* deltaTime;

			// 감속량이 현재 속도보다 크면 정확히 정지
			if (m_velocity.length() <= friction.length())
			{
				m_velocity = Vector2::Zero;
			}
			else
			{
				m_velocity += friction;
			}
		}

		// 최종 속도로 위치 변경
		Vector2 pos = transform->GetPosition();
		pos += m_velocity * deltaTime;
		transform->SetPosition(pos);

		// 이번 계산에 사용한 힘 제거
		m_force.clear();
	}

	void Rigidbody::LateUpdate()
	{
	}

	void Rigidbody::Render(HDC hdc)
	{
	}
}