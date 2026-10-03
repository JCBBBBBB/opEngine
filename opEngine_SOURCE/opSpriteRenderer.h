#pragma once
#include "opEntity.h"
#include "opComponent.h"
#include "opTexture.h"

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

		void SetTexture(graphics::Texture* texture) { m_texture = texture; }
		void SetSize(math::Vector2 size) { m_size = size; }

	private:
		graphics::Texture* m_texture;
		math::Vector2 m_size;
	};

}