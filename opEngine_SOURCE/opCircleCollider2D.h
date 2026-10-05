#pragma once
#include "opCollider.h"
namespace op
{
    class CircleCollider2D : public Collider
    {
    public:
        CircleCollider2D();
        ~CircleCollider2D() override;
        void Initialize() override;
        void Update() override;
        void LateUpdate() override;
        void Render(HDC hdc) override;
    private:
        float m_radius;
    };
}
