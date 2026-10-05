#pragma once
#include "opComponent.h"
#include "opTexture.h"

namespace op
{
	// 원본 이미지에서 타일 한 칸을 잘라 그리는 컴포넌트
	class TilemapRenderer : public Component
	{
	public:
		TilemapRenderer();
		~TilemapRenderer() override;

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;

		void SetTexture(graphics::Texture* texture)
		{
			m_texture = texture;
		}

		void SetSize(math::Vector2 size)
		{
			m_size = size;
		}

		// 원본 이미지에서 사용할 타일의 열과 행
		void SetIndex(math::Vector2 index)
		{
			m_index = index;
		}
		
		static math::Vector2 TileSize;       // 화면에 표시되는 한 칸 크기: 48 × 48
		static math::Vector2 OriginTileSize; // 원본 이미지의 한 칸 크기: 16 × 16
		static math::Vector2 SelectedIndex;  // 타일용 창에서 선택한 열과 행	
	private:
		math::Vector2 m_tileSize;       // 원본 타일 한 칸의 크기
		graphics::Texture* m_texture;   // 여러 타일이 들어 있는 원본 이미지
		math::Vector2 m_size;           // 표시 크기의 배율
		math::Vector2 m_index;
	};
}