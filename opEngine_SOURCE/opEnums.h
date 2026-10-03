#pragma once

namespace op::enums
{
	enum class LayerType
	{
		None,
		BackGround,
		Player,
		Max = 16,
	};

	enum class ResourceType
	{
		Texture,
		AudioClip,
		Prefab,
		End,
	};
}