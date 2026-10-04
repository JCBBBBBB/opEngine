#include "opRenderer.h"
#include "opSpriteRenderer.h"
#include "opGameObject.h"
#include "opTransform.h"
#include "opTexture.h"

namespace op
{
	SpriteRenderer::SpriteRenderer() : Component(enums::ComponentType::SpriteRenderer), m_texture(nullptr), m_size(Vector2::One)
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
		float rot = transform->GetRotation();
		Vector2 scale = transform->GetScale();

		pos = renderer::mainCamera->CalculatePosition(pos);

		if (m_texture->GetTextureType() == graphics::Texture::TextureType::Bmp)
		{
			TransparentBlt(hdc, pos.x, pos.y
				, m_texture->GetWidth() * m_size.x, m_texture->GetHeight() * m_size.y
				, m_texture->GetHdc(), 0, 0, m_texture->GetWidth(), m_texture->GetHeight()
				, RGB(255, 0, 255));
		}
		else if (m_texture->GetTextureType() == graphics::Texture::TextureType::Png)
		{
			Gdiplus::ImageAttributes imgAtt = {};
			imgAtt.SetColorKey(Gdiplus::Color(230, 230, 230), Gdiplus::Color(255, 255, 255));

			Gdiplus::Graphics graphics(hdc);

			graphics.TranslateTransform(pos.x, pos.y);
			graphics.RotateTransform(rot);
			graphics.TranslateTransform(-pos.x, -pos.y);


			graphics.DrawImage(m_texture->GetImage(), Gdiplus::Rect(pos.x, pos.y, m_texture->GetWidth() * m_size.x, m_texture->GetHeight() * m_size.y), 0, 0, m_texture->GetWidth(), m_texture->GetHeight(), Gdiplus::UnitPixel, nullptr);
		}
	}

	
}