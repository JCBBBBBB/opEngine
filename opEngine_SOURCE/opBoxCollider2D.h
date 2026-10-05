#pragma once
#include "opCollider.h"
namespace op
{
    class BoxCollider2D : public Collider
    {
    public:
        BoxCollider2D();
        ~BoxCollider2D() override;
        void Initialize() override;
        void Update() override;
        void LateUpdate() override;
        void Render(HDC hdc) override;
    };
}
