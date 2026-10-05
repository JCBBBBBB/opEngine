#include "opTexture.h"
#include "opApplication.h"
#include "opResources.h"

extern op::Application application;

namespace op::graphics
{
	Texture::Texture() : Resource(enums::ResourceType::Texture), m_type(TextureType::None), m_image(nullptr), m_bitmap(nullptr), m_hdc(nullptr), m_width(0), m_height(0), m_isAlpha(false) // 부모 Resource에 나는 Texture 리소스라고 전달
	{

	}

	Texture::~Texture()
	{
        delete m_image;
        if (m_hdc) { SelectObject(m_hdc, m_oldBitmap); DeleteDC(m_hdc); }
        if (m_bitmap) DeleteObject(m_bitmap);
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
				return E_FAIL; // 로딩 문제 시 반환하는 값
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

			m_oldBitmap = static_cast<HBITMAP>(SelectObject(m_hdc, m_bitmap));
		}

		else if (ext == L"png")
		{
			m_type = TextureType::Png;
			m_image = Gdiplus::Image::FromFile(path.c_str()); // 경로의 파일을 GDI+ 이미지로 읽음
			if (m_image == nullptr || m_image->GetLastStatus() != Gdiplus::Ok)
				{
					return E_FAIL;
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
        const std::wstring key = name + L"Image";
        if (auto* existing = Resources::Find<Texture>(key)) return existing;
        Texture* image = new Texture();
        image->m_type = TextureType::Bmp;
        image->SetName(key);
        image->m_width = width;
        image->m_height = height;
        HDC mainDC = application.GetHDC();
        image->m_bitmap = CreateCompatibleBitmap(mainDC, static_cast<int>(width), static_cast<int>(height));
        image->m_hdc = CreateCompatibleDC(mainDC);
        if (!image->m_bitmap || !image->m_hdc) { delete image; return nullptr; }
        image->m_oldBitmap = static_cast<HBITMAP>(SelectObject(image->m_hdc, image->m_bitmap));
        // 새 시트의 비트맵을 선택한 뒤 투명 키 색으로 배경을 초기화한다.
        HBRUSH brush = CreateSolidBrush(RGB(255, 0, 255));
        RECT area = { 0, 0, static_cast<LONG>(width), static_cast<LONG>(height) };
        FillRect(image->m_hdc, &area, brush);
        DeleteObject(brush);
        Resources::Insert(key, image);
        return image;
	}
}

