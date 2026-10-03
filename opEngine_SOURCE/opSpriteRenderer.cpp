#include "opSpriteRenderer.h"
#include "opGameObject.h"
#include "opTransform.h"
#include "opTexture.h"

namespace op
{
	SpriteRenderer::SpriteRenderer() : Component(), m_texture(nullptr), m_size(Vector2::One)
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
		if (m_texture == nullptr)
		{
			assert(false);
		}

		Transform* transform = GetOwner()->GetComponent<Transform>();
		Vector2 pos = transform->GetPosition();

		if (m_texture->GetTextureType() == graphics::Texture::TextureType::Bmp)
		{
			TransparentBlt(hdc, pos.x, pos.y
				, m_texture->GetWidth() * m_size.x, m_texture->GetHeight() * m_size.y
				, m_texture->GetHdc(), 0, 0, m_texture->GetWidth(), m_texture->GetHeight()
				, RGB(255, 0, 255));
		}
		else if (m_texture->GetTextureType()
			== graphics::Texture::TextureType::Png)
		{
			Gdiplus::Graphics graphcis(hdc);
			graphcis.DrawImage(m_texture->GetImage()
				, Gdiplus::Rect(pos.x, pos.y
					, m_texture->GetWidth() * m_size.x, m_texture->GetHeight() * m_size.y));
		}
	}

	
}