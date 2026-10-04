#pragma once
#include "opComponent.h"
#include "opAnimation.h"

namespace op
{	
	// 애니메이션 목록과 현재 애니메이션 관리
	class Animator : public Component
	{
	public:
		Animator();
		~Animator();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;

		void CreateAnimation(const std::wstring& name, graphics::Texture* spriteSheet, Vector2 leftTop, Vector2 size, Vector2 offset, UINT spriteLength, float duration);

		Animation* FindAnimation(const std::wstring& name);
		void PlayAnimation(const std::wstring& name, bool loop = true);

	private:
		std::map<std::wstring, Animation*> m_animations; // 이름으로 찾을 수 있는 애니메이션 목록
		Animation* m_activeAnimation; // 현재 재생 중인 애니메이션
		bool m_isLoop; // 반복 재생 여분
	};
}
