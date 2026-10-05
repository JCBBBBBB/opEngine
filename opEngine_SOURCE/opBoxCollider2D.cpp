#include "opBoxCollider2D.h"
#include "opGameObject.h"
#include "opTransform.h"
#include "opRenderer.h"
#include "opCamera.h"
namespace op
{
    BoxCollider2D::BoxCollider2D() : Collider() {}
    BoxCollider2D::~BoxCollider2D() {}
    void BoxCollider2D::Initialize() {}
    void BoxCollider2D::Update() {}
    void BoxCollider2D::LateUpdate() {}
    void BoxCollider2D::Render(HDC hdc)
    {
        if (!hdc || !GetOwner()) return;
        Transform* transform = GetOwner()->GetComponent<Transform>();
        if (!transform) return;
        Vector2 pos = transform->GetPosition() + GetOffset();
        // 그림과 충돌 영역에 같은 카메라 좌표 변환을 적용한다.
        if (renderer::mainCamera) pos = renderer::mainCamera->CalculatePosition(pos);
        HBRUSH oldBrush = static_cast<HBRUSH>(SelectObject(hdc, GetStockObject(NULL_BRUSH)));
        HPEN pen = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
        HPEN oldPen = static_cast<HPEN>(SelectObject(hdc, pen));
        Rectangle(hdc, static_cast<int>(pos.x), static_cast<int>(pos.y),
            static_cast<int>(pos.x + 100.0f), static_cast<int>(pos.y + 100.0f));
        SelectObject(hdc, oldPen);
        SelectObject(hdc, oldBrush);
        DeleteObject(pen);
    }
}
