#include "opTilemapRenderer.h"
#include "opGameObject.h"
#include "opTransform.h"
#include "opRenderer.h"
#include "opCamera.h"

namespace op
{
	math::Vector2 TilemapRenderer::TileSize =
		math::Vector2(48.0f, 48.0f);

	math::Vector2 TilemapRenderer::OriginTileSize =
		math::Vector2(16.0f, 16.0f);

	math::Vector2 TilemapRenderer::SelectedIndex =
		math::Vector2::One;

	TilemapRenderer::TilemapRenderer()
		: Component(enums::ComponentType::SpriteRenderer)
		, m_tileSize(16.0f, 16.0f)
		, m_texture(nullptr)
		, m_size(3.0f, 3.0f)
		, m_index(0.0f, 0.0f)
	{
		TileSize = m_tileSize * m_size;
		OriginTileSize = m_tileSize;
	}

	TilemapRenderer::~TilemapRenderer()
	{
	}

	void TilemapRenderer::Initialize()
	{
	}

	void TilemapRenderer::Update()
	{
	}

	void TilemapRenderer::LateUpdate()
	{
	}

	void TilemapRenderer::Render(HDC hdc)
	{
		if (!hdc || !m_texture || !GetOwner())
		{
			return;
		}

		Transform* transform = GetOwner()->GetComponent<Transform>();

		if (!transform)
		{
			return;
		}

		Vector2 pos = transform->GetPosition();
		Vector2 scale = transform->GetScale();

		if (renderer::mainCamera)
		{
			pos = renderer::mainCamera->CalculatePosition(pos);
		}

		// 이번 커밋에서는 BMP 타일만 처리한다.
		if (m_texture->GetTextureType()
			!= graphics::Texture::TextureType::Bmp)
		{
			return;
		}

		HDC imageHdc = m_texture->GetHdc();

		if (!imageHdc)
		{
			return;
		}

		// 화면에서 그릴 위치
		const int destX = static_cast<int>(pos.x);
		const int destY = static_cast<int>(pos.y);

		// 원본 타일 크기 × 표시 배율 × Transform 배율
		const int destWidth =
			static_cast<int>(m_tileSize.x * m_size.x * scale.x);

		const int destHeight =
			static_cast<int>(m_tileSize.y * m_size.y * scale.y);

		// 원본 이미지에서 잘라올 위치와 크기
		const int sourceX =
			static_cast<int>(m_index.x * m_tileSize.x);

		const int sourceY =
			static_cast<int>(m_index.y * m_tileSize.y);

		const int sourceWidth = static_cast<int>(m_tileSize.x);
		const int sourceHeight = static_cast<int>(m_tileSize.y);

		if (destWidth <= 0 || destHeight <= 0)
		{
			return;
		}

		if (m_texture->IsAlpha())
		{
			BLENDFUNCTION func = {};
			func.BlendOp = AC_SRC_OVER;
			func.BlendFlags = 0;
			func.AlphaFormat = AC_SRC_ALPHA;
			func.SourceConstantAlpha = 255;

			AlphaBlend(
				hdc,
				destX, destY,
				destWidth, destHeight,
				imageHdc,
				sourceX, sourceY,
				sourceWidth, sourceHeight,
				func);
		}
		else
		{
			// 자홍색은 투명하게 처리한다.
			TransparentBlt(
				hdc,
				destX, destY,
				destWidth, destHeight,
				imageHdc,
				sourceX, sourceY,
				sourceWidth, sourceHeight,
				RGB(255, 0, 255));
		}
	}
}