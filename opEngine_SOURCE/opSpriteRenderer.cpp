#include "opSpriteRenderer.h"
#include "opGameObject.h"
#include "opTransform.h"

namespace op
{
	SpriteRenderer::SpriteRenderer() : m_image(nullptr), m_width(0), m_height(0)
	{

	}

	SpriteRenderer::~SpriteRenderer()
	{

	}

	void SpriteRenderer::Initialize()
	{

	}

	void SpriteRenderer::Update()
	{

	}

	void SpriteRenderer::LateUpdate()
	{

	}

	void SpriteRenderer::Render(HDC hdc)
	{
		Transform* tr = GetOwner()->GetComponent<Transform>(); // 게임 객체에서 위치 담당 부품을 찾는다
		Vector2 pos = tr->GetPosition(); // 부품에서 위치를 읽는다

		Gdiplus::Graphics graphics(hdc); // 전달받은 hdc에 그림을 그릴 도구 준비
		graphics.DrawImage(m_image, Gdiplus::Rect(pos.x, pos.y, m_width, m_height)); // 준비된 이미지를 지정한 위치와 크기로 그린다
	}

	void SpriteRenderer::ImageLoad(const std::wstring& path)
	{
		m_image = Gdiplus::Image::FromFile(path.c_str()); // 파일 경로 사용해서 이미지 읽기
		m_width = m_image->GetWidth();
		m_height = m_image->GetHeight();
	}
}