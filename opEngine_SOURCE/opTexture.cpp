#include "opTexture.h"
#include "opApplication.h"
#include "opResources.h"

extern op::Application application;

namespace op::graphics
{
	Texture::Texture() : Resource(enums::ResourceType::Texture), m_isAlpha(false) // 부모 Resource에 나는 Texture 리소스라고 전달
	{

	}

	Texture::~Texture()
	{

	}

	HRESULT Texture::Load(const std::wstring& path)
	{
		std::wstring ext = path.substr(path.find_last_of(L".") + 1); // png인지 bmp인지 가져옴

		// bmp 일때
		if (ext == L"bmp")
		{
			m_type = TextureType::Bmp;
			m_bitmap = (HBITMAP)LoadImageW(nullptr, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION); // 경로 파일을 BMP 비트맵으로 읽음

			if (m_bitmap == nullptr)
			{
				return S_FALSE; // 로딩 문제 시 반환하는 값
			}

			BITMAP info = {}; // 이미지 정보를 받을 구조체 0으로 초기화
			GetObject(m_bitmap, sizeof(BITMAP), &info); // 비트맵의 크기 등 정보를 info에 채움

			m_width = info.bmWidth; // 너비 저장
			m_height = info.bmHeight; // 높이 저장

			if (info.bmBitsPixel == 32)
				m_isAlpha = true;
			else if (info.bmBitsPixel == 24)
				m_isAlpha = false;

			HDC mainDC = application.GetHDC(); // 그 DC와 호환되는 메모리 DC 생성
			m_hdc = CreateCompatibleDC(mainDC);

			HBITMAP oldBitmap = (HBITMAP)SelectObject(m_hdc, m_bitmap);
			DeleteObject(oldBitmap);
		}

		else if (ext == L"png")
		{
			m_type = TextureType::Png;
			m_image = Gdiplus::Image::FromFile(path.c_str()); // 경로의 파일을 GDI+ 이미지로 읽음
			if (m_image == nullptr)
				{
					return S_FALSE;
				}
			m_width = m_image->GetWidth(); // 너비 저장
			m_height = m_image->GetHeight(); // 높이 저장
		}

		return S_OK;
	}

	Texture* Texture::Create(
		const std::wstring& name,
		UINT width,
		UINT height)
	{
		Texture* image = Resources::Find<Texture>(name);

		if (image)
			return image;

		image = new Texture();
		image->m_type = TextureType::Bmp;
		image->SetName(name);
		image->SetWidth(width);
		image->SetHeight(height);

		HDC hdc = application.GetHDC();
		HWND hwnd = application.GetHwnd();

		image->m_bitmap = CreateCompatibleBitmap(
			hdc, width, height);

		image->m_hdc = CreateCompatibleDC(hdc);

		HBITMAP oldBitmap = (HBITMAP)SelectObject(
			image->m_hdc, image->m_bitmap);

		DeleteObject(oldBitmap);

		Resources::Insert(name + L"Image", image);

		return image;
	}
}

