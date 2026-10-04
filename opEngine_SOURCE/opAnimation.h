#pragma once
#include "opResource.h"
#include "opTexture.h"

namespace op
{
	//Animation -> 어느 그림을 몇 초동안 보여줄지와 현재 그림 번호
	class Animation : public Resource
	{
	public:
		struct Sprite // 프레임 한 장을 표시하기 위한 정보
		{
			Vector2 leftTop; // 원본 이미지에서 잘라 쓸 영역의 왼쪽 위
			Vector2 size; // 잘라 쓸 영역의 너비/높이

			Vector2 offset; // 위치 보정값
			float duration; // 이 프레임을 유지할 시간

			Sprite() : leftTop(Vector2::Zero), size(Vector2::Zero), offset(Vector2::Zero), duration(0.0f)
			{

			}
		};

		Animation();
		~Animation();

		HRESULT Load(const std::wstring& path) override;

		void Update();
		void Render(HDC hdc);

		void CreateAnimation(const std::wstring& name, graphics::Texture* spriteSheet, Vector2 leftTop, Vector2 size, Vector2 offset, UINT spriteLength, float duration);
		void Reset();
		
		bool IsComplete() { return m_isComplete; }
		void SetAnimator(class Animator* animator) { m_animator = animator; }

	private:
		class Animator* m_animator; // 이 Animation을 관리하는 Animator
		graphics::Texture* m_texture; // 프레임들을 가져올 원본 스프라이트 시트

		std::vector<Sprite> m_animationSheet; // 각 프레임의 원본 영역, 크기, 유지 시간 정보 목록
		int m_index; // 현재 표시할 프레임 정보 (0,1,2,3)
		float m_time; // 현재 프레임에서 흐른 시간
		bool m_isComplete; // 마지막 프레임의 유지 시간까지 끝났는지
	};
}
