#pragma once
#include "opResource.h"


namespace op::graphics
{
	//Texture->그림들이 들어 있는 원본 이미지
	class Texture : public Resource
	{
	public:
		enum class TextureType
		{
			Bmp,
			Png,
			None,
		};

		Texture();
		~Texture();

		virtual HRESULT Load(const std::wstring& path) override;
		static Texture* Create(const std::wstring& name, UINT width, UINT height);

		void SetWidth(UINT width)
		{
			m_width = width;
		}

		void SetHeight(UINT height)
		{
			m_height = height;
		}
		UINT GetWidth() { return m_width; }
		UINT GetHeight() { return m_height; }
		HDC GetHdc() { return m_hdc; }
		TextureType GetTextureType() { return m_type; }
		Gdiplus::Image* GetImage() { return m_image; }
		bool IsAlpha() { return m_isAlpha; }

	private:
		TextureType m_type; // bmp인지 png인지 구분
		Gdiplus::Image* m_image; // png를 읽은 gdi+ 이미지
		HBITMAP m_oldBitmap = nullptr;
		HBITMAP m_bitmap; // bmp 비트맵
		HDC m_hdc; // bmp 복사할 때 사용할 메모리 dc

		// 이미지 원본 크기
		UINT m_width; 
		UINT m_height;

		bool m_isAlpha;
	};
}
