#include "opCollider.h"
#include "opGameObject.h"
#include "opScript.h"
#include "opCollisionManager.h"
namespace op
{
    UINT32 Collider::CollisionID = 1;
    Collider::Collider() : Component(enums::ComponentType::Collider), m_id(CollisionID++),
        m_size(Vector2::One), m_offset(Vector2::Zero)
    {
        CollisionManager::RegisterCollider(this);
    }
    Collider::~Collider() { CollisionManager::ForgetCollider(m_id); }
    void Collider::Initialize() {}
    void Collider::Update() {}
    void Collider::LateUpdate() {}
    void Collider::Render(HDC) {}
    void Collider::OnCollisionEnter(Collider* other)
    {
        if (GetOwner())
            if (auto* script = GetOwner()->GetComponent<Script>()) script->OnCollisionEnter(other);
    }
    void Collider::OnCollisionStay(Collider* other)
    {
        if (GetOwner())
            if (auto* script = GetOwner()->GetComponent<Script>()) script->OnCollisionStay(other);
    }
    void Collider::OnCollisionExit(Collider* other)
    {
        if (GetOwner())
            if (auto* script = GetOwner()->GetComponent<Script>()) script->OnCollisionExit(other);
    }
}
