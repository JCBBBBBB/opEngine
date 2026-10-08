#pragma once

namespace op::enums
{
	enum class ComponentType
	{
		Transform,
		Collider,
		Rigidbody,
		Script,
		SpriteRenderer,
		Animator,
		Camera,
		AudioListener,
		AudioSource,
		End,
	};


	enum class LayerType
	{
		None,
		BackGround,
		Tile,
		Animal,
		Player,
		Floor,
		Particle,
		Brick,
		Max = 16,
	};

    enum class ColliderType
    {
        Circle2D,
        Rect2D,
        End,
    };

	enum class ResourceType
	{
		Texture, // 이미지
		AudioClip, // 소리
		Animation,
		Prefab, // 재사용할 오브젝트
		End,
	};

	enum class UIType
	{
		HpBar,
		Button,
		End,
	};
}