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
			// 가속도 계산에서 질량으로 나누므로 양수만 허용
			if (mass > 0.0f)
			{
				m_mass = mass;
			}
		}

		void AddForce(math::Vector2 force)
		{
			m_force = force;
		}

	private:
		float m_mass;     // 질량
		float m_friction; // 감속에 사용할 값

		math::Vector2 m_force;        // 적용할 힘
		math::Vector2 m_acceleration; // 가속도
		math::Vector2 m_velocity;     // 속도
		math::Vector2 m_gravity;      // 이번 단계에서는 아직 사용하지 않음
	};
}