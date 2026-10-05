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
        if (m_texture == nullptr || GetOwner() == nullptr)
            return;

        Transform* transform =
            GetOwner()->GetComponent<Transform>();

        Vector2 pos = transform->GetPosition();
        float rot = transform->GetRotation();
        Vector2 scale = transform->GetScale();

        if (renderer::mainCamera) pos = renderer::mainCamera->CalculatePosition(pos);

        if (m_texture->GetTextureType()
            == graphics::Texture::TextureType::Bmp)
        {
            if (m_texture->IsAlpha())
            {
                BLENDFUNCTION func = {};
                func.BlendOp = AC_SRC_OVER;
                func.BlendFlags = 0;
                func.AlphaFormat = AC_SRC_ALPHA;
                func.SourceConstantAlpha = 255;

                AlphaBlend(
                    hdc,
                    static_cast<int>(pos.x),
                    static_cast<int>(pos.y),
                    static_cast<int>(m_texture->GetWidth() * m_size.x * scale.x),
                    static_cast<int>(m_texture->GetHeight() * m_size.y * scale.y),
                    m_texture->GetHdc(),
                    0,
                    0,
                    m_texture->GetWidth(),
                    m_texture->GetHeight(),
                    func);
            }
            else
            {
                TransparentBlt(
                    hdc,
                    static_cast<int>(pos.x),
                    static_cast<int>(pos.y),
                    static_cast<int>(m_texture->GetWidth() * m_size.x * scale.x),
                    static_cast<int>(m_texture->GetHeight() * m_size.y * scale.y),
                    m_texture->GetHdc(),
                    0,
                    0,
                    m_texture->GetWidth(),
                    m_texture->GetHeight(),
                    RGB(255, 0, 255));
            }
        }
        else if (m_texture->GetTextureType()
            == graphics::Texture::TextureType::Png)
        {
            Gdiplus::ImageAttributes imgAtt = {};

            imgAtt.SetColorKey(
                Gdiplus::Color(230, 230, 230),
                Gdiplus::Color(255, 255, 255));

            Gdiplus::Graphics graphics(hdc);

            graphics.TranslateTransform(pos.x, pos.y);
            graphics.RotateTransform(rot);
            graphics.TranslateTransform(-pos.x, -pos.y);

            graphics.DrawImage(
                m_texture->GetImage(),
                Gdiplus::Rect(
                    static_cast<int>(pos.x),
                    static_cast<int>(pos.y),
                    static_cast<int>(m_texture->GetWidth() * m_size.x * scale.x),
                    static_cast<int>(m_texture->GetHeight() * m_size.y * scale.y)),
                0,
                0,
                m_texture->GetWidth(),
                m_texture->GetHeight(),
                Gdiplus::UnitPixel,
                nullptr/*&imgAtt*/);
        }
    }

	
}