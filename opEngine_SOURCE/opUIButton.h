#pragma once
#include "opUIBase.h"
#include "opTexture.h"

namespace op
{
	class UIButton : public UIBase
	{
	public:
		UIButton();
		~UIButton() override;

		void OnInit() override;
		void OnActive() override;
		void OnInActive() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnRender(HDC hdc) override;
		void OnClear() override;

		virtual void ButtonClick();

	private:
		graphics::Texture* m_texture; // 원본에 있지만 아직 사용하지 않음
		Event m_onClick;             // 클릭했을 때 실행할 함수
	};
}