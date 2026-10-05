#include "opRigidbody.h"
#include "opTime.h"
#include "opTransform.h"
#include "opGameObject.h"

namespace op
{
	Rigidbody::Rigidbody()
		: Component(enums::ComponentType::Rigidbody)
		, m_ground(false)
		, m_mass(1.0f)
		, m_friction(10.0f)
		, m_force(Vector2::Zero)
		, m_acceleration(Vector2::Zero)
		, m_velocity(Vector2::Zero)
		, m_limitedVelocity(200.0f, 1000.0f)
		, m_gravity(0.0f, 800.0f)
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

		// 힘으로 인한 가속도와 속도 계산
		m_acceleration = m_force / m_mass;
		m_velocity += m_acceleration * deltaTime;

		// 중력 방향: (0, 800)을 정규화하면 (0, 1)
		Vector2 gravityDirection = m_gravity;
		gravityDirection.normalize();

		if (m_ground)
		{
			// 바닥에서는 중력 방향의 속도를 제거한다.
			const float dot =
				Vector2::Dot(m_velocity, gravityDirection);

			m_velocity -= gravityDirection * dot;
		}
		else
		{
			// 공중에서는 아래쪽으로 계속 가속한다.
			m_velocity += m_gravity * deltaTime;
		}

		// 속도를 중력 방향과 옆 방향으로 나눈다.
		const float dot =
			Vector2::Dot(m_velocity, gravityDirection);

		Vector2 gravityVelocity = gravityDirection * dot;
		Vector2 sideVelocity = m_velocity - gravityVelocity;

		// 중력 방향 최대 속도 제한
		if (gravityVelocity.length() > m_limitedVelocity.y)
		{
			gravityVelocity.normalize();
			gravityVelocity *= m_limitedVelocity.y;
		}

		// 옆 방향 최대 속도 제한
		if (sideVelocity.length() > m_limitedVelocity.x)
		{
			sideVelocity.normalize();
			sideVelocity *= m_limitedVelocity.x;
		}

		m_velocity = gravityVelocity + sideVelocity;

		// 원본과 같이 현재 속도의 반대 방향으로 감속
		if (!(m_velocity == Vector2::Zero))
		{
			Vector2 friction = -m_velocity;

			friction =
				friction.normalize()
				* m_friction
				* m_mass
				* deltaTime;

			if (m_velocity.length() <= friction.length())
			{
				m_velocity = Vector2::Zero;
			}
			else
			{
				m_velocity += friction;
			}
		}

		Vector2 pos = transform->GetPosition();
		pos += m_velocity * deltaTime;
		transform->SetPosition(pos);

		m_force.clear();
	}

	void Rigidbody::LateUpdate()
	{
	}

	void Rigidbody::Render(HDC hdc)
	{
	}
}