#pragma once

namespace op::enums
{
	enum class ComponentType
	{
		Transform,
		Collider,
		Script,
		SpriteRenderer,
		Animator,
		Camera,
		End,
	};


	enum class LayerType
	{
		None,
		BackGround,
		Animal,
		Player,
		Particle,
		Max = 16,
	};

	enum class ResourceType
	{
		Texture, // 이미지
		AudioClip, // 소리
		Animation,
		Prefab, // 재사용할 오브젝트
		End,
	};
}