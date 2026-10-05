#pragma once
#include "opComponent.h"

namespace op
{
    // 충돌 컴포넌트의 공통 부모. offset은 오브젝트 위치로부터의 보정값.
    class Collider : public Component
    {
    public:
        Collider();
        ~Collider() override;
        void Initialize() override;
        void Update() override;
        void LateUpdate() override;
        void Render(HDC hdc) override;
        virtual void OnCollisionEnter(Collider* other);
        virtual void OnCollisionStay(Collider* other);
        virtual void OnCollisionExit(Collider* other);
        UINT32 GetID() const { return m_id; }
        Vector2 GetSize() const { return m_size; }
        void SetSize(Vector2 size) { m_size = size; }
        Vector2 GetOffset() const { return m_offset; }
        void SetOffset(Vector2 offset) { m_offset = offset; }
    private:
        static UINT32 CollisionID;
        const UINT32 m_id;
        Vector2 m_size;
        Vector2 m_offset;
    };
}
