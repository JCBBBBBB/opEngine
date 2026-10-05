#include "opCircleCollider2D.h"
#include "opGameObject.h"
#include "opTransform.h"
#include "opRenderer.h"
#include "opCamera.h"
namespace op
{
    CircleCollider2D::CircleCollider2D() : Collider(enums::ColliderType::Circle2D) {}
    CircleCollider2D::~CircleCollider2D() {}
    void CircleCollider2D::Initialize() {}
    void CircleCollider2D::Update() {}
    void CircleCollider2D::LateUpdate() {}
    void CircleCollider2D::Render(HDC hdc)
    {
        if (!hdc || !GetOwner()) return;
        Transform* transform = GetOwner()->GetComponent<Transform>();
        if (!transform) return;
        Vector2 pos = transform->GetPosition() + GetOffset();
        Vector2 size = GetSize() * 100.0f;
        if (renderer::mainCamera) pos = renderer::mainCamera->CalculatePosition(pos);
        HBRUSH oldBrush = static_cast<HBRUSH>(SelectObject(hdc, GetStockObject(NULL_BRUSH)));
        HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
        HPEN oldPen = static_cast<HPEN>(SelectObject(hdc, pen));
        Ellipse(hdc, static_cast<int>(pos.x), static_cast<int>(pos.y),
            static_cast<int>(pos.x + size.x), static_cast<int>(pos.y + size.y));
        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBrush);
        DeleteObject(pen);
    }
}
