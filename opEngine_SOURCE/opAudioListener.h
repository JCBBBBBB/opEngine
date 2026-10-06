#pragma once
#include "opComponent.h"

namespace op
{
	class AudioListener : public Component
	{
	public:
		AudioListener();
		~AudioListener() override;

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;
	};
}