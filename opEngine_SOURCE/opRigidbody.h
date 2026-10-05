#pragma once
#include "opComponent.h"

namespace op
{
	class Rigidbody : public Component
	{
	public:
		Rigidbody();
		~Rigidbody() override;

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;

		void SetMass(float mass)
		{
			if (mass > 0.0f)
			{
				m_mass = mass;
			}
		}

		void AddForce(math::Vector2 force)
		{
			m_force = force;
		}

		void SetGround(bool ground)
		{
			m_ground = ground;
		}

		bool IsGround() const
		{
			return m_ground;
		}

		void SetVelocity(math::Vector2 velocity)
		{
			m_velocity = velocity;
		}

		math::Vector2 GetVelocity() const
		{
			return m_velocity;
		}

	private:
		bool m_ground;   // 바닥에 서 있는지
		float m_mass;
		float m_friction;

		math::Vector2 m_force;
		math::Vector2 m_acceleration;
		math::Vector2 m_velocity;
		math::Vector2 m_limitedVelocity; // 가로/중력 방향의 최대 속도
		math::Vector2 m_gravity;
	};
}