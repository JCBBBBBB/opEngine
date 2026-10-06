#pragma once
#include "opUIBase.h"
#include "opTexture.h"

namespace op
{
	class UIHUD : public UIBase
	{
	public:
		UIHUD();
		~UIHUD() override;

		void OnInit() override;
		void OnActive() override;
		void OnInActive() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnRender(HDC hdc) override;
		void OnClear() override;

	private:
		graphics::Texture* m_texture;
	};
}