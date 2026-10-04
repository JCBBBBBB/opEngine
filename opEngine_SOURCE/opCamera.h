#pragma once
#include "opComponent.h"

namespace op
{
	using namespace op::math;
	class Camera : public Component
	{
	public:
		Vector2 CalculatePosition(Vector2 pos) { return pos - m_distance; } // 화면에 그릴 위치 계산

		Camera();
		~Camera();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;

		void SetTarget(GameObject* target)
		{
			m_target = target;
		}

	private:
		class GameObject* m_target; // 추적 대상으로 사용할 오브젝트 주소
		Vector2 m_distance; // 게임 좌표를 화면 좌표로 바꿀 때 뺼 값
		Vector2 m_resolution; // Application에서 가져온 너비/높이
		Vector2 m_lookPosition; // 카메라가 바라보는 중심 좌표
	};
}
