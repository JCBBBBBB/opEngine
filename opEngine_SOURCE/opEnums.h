#pragma once

namespace op::enums
{
	enum class ComponentType
	{
		Transform,
		SpriteRenderer,
		Animator,
		Script,
		Camera,
		End,
	};


	enum class LayerType
	{
		None,
		BackGround,
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