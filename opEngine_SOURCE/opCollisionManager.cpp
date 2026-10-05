#include "opCollisionManager.h"
#include "opScene.h"
#include "opSceneManager.h"
#include "opGameObject.h"
namespace op
{
    std::bitset<CollisionManager::LayerCount> CollisionManager::m_collisionLayerMatrix[LayerCount] = {};
    void CollisionManager::Initialize()
    {
        for (auto& row : m_collisionLayerMatrix) row.reset();
    }
    void CollisionManager::Update()
    {
        Scene* scene = SceneManager::GetActiveScene();
        if (!scene) return;
        for (size_t row = 0; row < LayerCount; ++row)
            for (size_t col = row; col < LayerCount; ++col)
                if (m_collisionLayerMatrix[row][col])
                    LayerCollision(scene, static_cast<enums::LayerType>(row), static_cast<enums::LayerType>(col));
    }
    void CollisionManager::LateUpdate() {}
    void CollisionManager::Render(HDC) {}
    void CollisionManager::CollisionLayerCheck(enums::LayerType left, enums::LayerType right, bool enable)
    {
        size_t row = static_cast<size_t>(left), col = static_cast<size_t>(right);
        if (row >= LayerCount || col >= LayerCount) return;
        if (row > col) std::swap(row, col);
        m_collisionLayerMatrix[row].set(col, enable);
    }
    bool CollisionManager::IsLayerEnabled(enums::LayerType left, enums::LayerType right)
    {
        size_t row = static_cast<size_t>(left), col = static_cast<size_t>(right);
        if (row >= LayerCount || col >= LayerCount) return false;
        if (row > col) std::swap(row, col);
        return m_collisionLayerMatrix[row][col];
    }
    void CollisionManager::LayerCollision(Scene* scene, enums::LayerType left, enums::LayerType right)
    {
        if (!scene) return;
        // 콜백에서 새 오브젝트를 추가해도 반복자가 무효화되지 않도록 목록을 복사한다.
        const auto lefts = scene->GetLayer(left)->GetGameObjects();
        const auto rights = scene->GetLayer(right)->GetGameObjects();
        for (size_t i = 0; i < lefts.size(); ++i)
        {
            GameObject* leftObject = lefts[i];
            if (!leftObject || !leftObject->IsActive()) continue;
            Collider* leftCollider = leftObject->GetComponent<Collider>();
            if (!leftCollider) continue;
            const size_t start = left == right ? i + 1 : 0;
            for (size_t j = start; j < rights.size(); ++j)
            {
                GameObject* rightObject = rights[j];
                if (!leftObject->IsActive()) break;
                if (!rightObject || rightObject == leftObject || !rightObject->IsActive()) continue;
                Collider* rightCollider = rightObject->GetComponent<Collider>();
                if (rightCollider) ColliderCollision(leftCollider, rightCollider);
            }
        }
    }
    void CollisionManager::ColliderCollision(Collider*, Collider*)
    {
        // 다음 원본 커밋에서 교차 검사와 Enter/Stay/Exit를 구현한다.
    }
}
