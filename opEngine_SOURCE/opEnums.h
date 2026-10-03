#pragma once

namespace op::enums
{
	enum class ComponentType
	{
		Transform,
		SpriteRenderer,
		Script,
		Camera,
		End,
	};


	enum class LayerType
	{
		None,
		BackGround,
		Player,
		Max = 16,
	};

	enum class ResourceType
	{
		Texture, // 이미지
		AudioClip, // 소리
		Prefab, // 재사용할 오브젝트
		End,
	};
}