#include "opCollider.h"
namespace op
{
    Collider::Collider() : Component(enums::ComponentType::Collider), m_offset(Vector2::Zero) {}
    Collider::~Collider() {}
    void Collider::Initialize() {}
    void Collider::Update() {}
    void Collider::LateUpdate() {}
    void Collider::Render(HDC) {}
}
