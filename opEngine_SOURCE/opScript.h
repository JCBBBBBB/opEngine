#pragma once
#include "opComponent.h"

namespace op
{
	class Collider;
	class Script : public Component 
	{
	public:
		Script();
		~Script();

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;
        virtual void OnCollisionEnter(Collider* other);
        virtual void OnCollisionStay(Collider* other);
        virtual void OnCollisionExit(Collider* other);
	};
}
