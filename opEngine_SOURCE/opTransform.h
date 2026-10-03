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

	private:
		Vector2 m_position;
	};
}
