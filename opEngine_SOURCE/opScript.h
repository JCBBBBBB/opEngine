#pragma once
#include "opComponent.h"

namespace op
{
	class Script : public Component 
	{
	public:
		Script();
		~Script();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;
	};
}
