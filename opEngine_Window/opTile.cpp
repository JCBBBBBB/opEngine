#include "opTile.h"
#include "..\\opEngine_SOURCE\\opTransform.h"
#include "..\\opEngine_SOURCE\\opTilemapRenderer.h"

namespace op
{
	void Tile::Initialize()
	{
		GameObject::Initialize();
	}

	void Tile::Update()
	{
		GameObject::Update();
	}

	void Tile::LateUpdate()
	{
		GameObject::LateUpdate();
	}

	void Tile::Render(HDC hdc)
	{
		GameObject::Render(hdc);
	}

	void Tile::SetPosition(int x, int y)
	{
		Transform* transform = GetComponent<Transform>();

		if (!transform)
		{
			return;
		}

		Vector2 pos;
		pos.x = static_cast<float>(x) * TilemapRenderer::TileSize.x;
		pos.y = static_cast<float>(y) * TilemapRenderer::TileSize.y;

		transform->SetPosition(pos);
	}
}