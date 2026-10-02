#pragma once
#include "opEntity.h"
#include "opComponent.h"

namespace op
{
	class SpriteRenderer : public Component
	{
	public:
		SpriteRenderer();
		~SpriteRenderer();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;

		void ImageLoad(const std::wstring& path);

	private:
		Gdiplus::Image* m_image;
		UINT m_width;
		UINT m_height;
	};

}