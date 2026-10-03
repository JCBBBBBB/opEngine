#include "opTexture.h"
#include "opApplication.h"

extern op::Application application;

namespace op::graphics
{
	Texture::Texture() : Resource(enums::ResourceType::Texture)
	{

	}

	Texture::~Texture()
	{

	}

	HRESULT Texture::Load(const std::wstring& path)
	{
		std::wstring ext = path.substr(path.find_last_of(L".") + 1);

		// bmp ÀÏ¶§
		if (ext == L"bmp")
		{
			m_type = TextureType::Bmp;
			m_bitmap = (HBITMAP)LoadImageW(nullptr, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);

			if (m_bitmap == nullptr)
			{
				return S_FALSE;
			}

			BITMAP info = {};
			GetObject(m_bitmap, sizeof(BITMAP), &info);

			m_width = info.bmWidth;
			m_height = info.bmHeight;

			HDC mainDC = application.GetHDC();
			m_hdc = CreateCompatibleDC(mainDC);

			HBITMAP oldBitmap = (HBITMAP)SelectObject(m_hdc, m_bitmap);
			DeleteObject(oldBitmap);
		}

		else if (ext == L"png")
		{
			m_type = TextureType::Png;
			m_image = Gdiplus::Image::FromFile(path.c_str());
			if (m_image == nullptr)
				{
					return S_FALSE;
				}
			m_width = m_image->GetWidth();
			m_height = m_image->GetHeight();
		}

		return S_OK;
	}

}

