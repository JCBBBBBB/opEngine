#pragma once

#include "opEntity.h"
#include "opComponent.h"

namespace op
{
	using namespace math;

	class Transform : public Component
	{
	public:
		Transform();
		~Transform();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;

		void SetPosition(Vector2 pos) { m_position.x = pos.x; m_position.y = pos.y; }
		Vector2 GetPosition() { return m_position; }

		void SetScale(Vector2 scale) { m_scale = scale; }
		Vector2 GetScale() { return m_scale; }

		void SetRotation(float rotation) { m_rotation = rotation; }
		float GetRotation() { return m_rotation; }

	private:
		Vector2 m_position;
		Vector2 m_scale;
		float m_rotation;
	};
}
