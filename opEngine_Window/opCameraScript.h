#pragma once
#include "..\\opEngine_SOURCE\\opScript.h"

namespace op
{
	class CameraScript : public Script
	{
	public:
		CameraScript();
		~CameraScript() override;

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;
	};
}