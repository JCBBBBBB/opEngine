#include "opCollisionManager.h"
#include "opScene.h"
#include "opSceneManager.h"
#include "opGameObject.h"
#include "opTransform.h"
namespace op
{
    std::bitset<CollisionManager::LayerCount> CollisionManager::m_collisionLayerMatrix[LayerCount] = {};
    std::unordered_map<UINT64, bool> CollisionManager::m_collisionMap;
    std::unordered_map<UINT32, Collider*> CollisionManager::m_liveColliders;
    std::unordered_set<UINT64> CollisionManager::m_checkedPairs;
    Scene* CollisionManager::m_collisionScene = nullptr;
    void CollisionManager::Initialize()
    {
        Reset();
        for (auto& row : m_collisionLayerMatrix) row.reset();
    }
    void CollisionManager::Update()
    {
        Scene* scene = SceneManager::GetActiveScene();
        if (m_collisionScene != scene) { Reset(); m_collisionScene = scene; }
        if (!scene) return;
        m_checkedPairs.clear();
        for (size_t row = 0; row < LayerCount; ++row)
            for (size_t col = row; col < LayerCount; ++col)
            {
                if (m_collisionLayerMatrix[row][col])
                    LayerCollision(scene, static_cast<enums::LayerType>(row), static_cast<enums::LayerType>(col));
                // 이벤트 안에서 씬을 전환했다면 이전 씬의 검사를 중단한다.
                if (SceneManager::GetActiveScene() != scene) { Reset(); return; }
            }
        std::vector<UINT64> exits;
        for (const auto& pair : m_collisionMap)
            if (m_checkedPairs.find(pair.first) == m_checkedPairs.end()) exits.push_back(pair.first);
        for (UINT64 pair : exits)
        {
            if (m_collisionMap.erase(pair)) Dispatch(pair, 2);
            if (SceneManager::GetActiveScene() != scene) { Reset(); return; }
        }
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
        if (!scene || static_cast<size_t>(left) >= LayerCount || static_cast<size_t>(right) >= LayerCount) return;
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
                if (SceneManager::GetActiveScene() != scene) return;
                // 콜백이 현재 충돌 컴포넌트를 교체할 수도 있다.
                if (leftObject->GetComponent<Collider>() != leftCollider) break;
            }
        }
    }
    void CollisionManager::ColliderCollision(Collider* left, Collider* right)
    {
        if (!left || !right || left == right || !left->GetOwner() || !right->GetOwner()) return;
        if (!left->GetOwner()->IsActive() || !right->GetOwner()->IsActive()) return;
        UINT64 pair = PairID(left->GetID(), right->GetID());
        m_checkedPairs.insert(pair);
        const bool previous = m_collisionMap.find(pair) != m_collisionMap.end();
        if (Intersect(left, right))
        {
            m_collisionMap[pair] = true;
            Dispatch(pair, previous ? 1 : 0);
        }
        else if (previous)
        {
            m_collisionMap.erase(pair);
            Dispatch(pair, 2);
        }
	}
    UINT64 CollisionManager::PairID(UINT32 left, UINT32 right)
    {
        if (left > right) std::swap(left, right);
        return (static_cast<UINT64>(left) << 32) | right;
    }
    void CollisionManager::RegisterCollider(Collider* collider)
    {
        if (collider) m_liveColliders[collider->GetID()] = collider;
    }
    void CollisionManager::ForgetCollider(UINT32 id)
    {
        m_liveColliders.erase(id);
        for (auto it = m_collisionMap.begin(); it != m_collisionMap.end(); )
        {
            if (static_cast<UINT32>(it->first >> 32) == id || static_cast<UINT32>(it->first) == id)
                it = m_collisionMap.erase(it);
            else ++it;
        }
    }
    void CollisionManager::Reset()
    {
        m_collisionMap.clear();
        m_checkedPairs.clear();
        m_collisionScene = nullptr;
    }
    void CollisionManager::Dispatch(UINT64 pair, int event)
    {
        const UINT32 leftID = static_cast<UINT32>(pair >> 32);
        const UINT32 rightID = static_cast<UINT32>(pair);
        auto left = m_liveColliders.find(leftID);
        auto right = m_liveColliders.find(rightID);
        if (left == m_liveColliders.end() || right == m_liveColliders.end()) return;
        Scene* scene = SceneManager::GetActiveScene();
        if (event == 0) left->second->OnCollisionEnter(right->second);
        else if (event == 1) left->second->OnCollisionStay(right->second);
        else left->second->OnCollisionExit(right->second);
        if (SceneManager::GetActiveScene() != scene) return;
        // 첫 콜백에서 컴포넌트가 교체/삭제될 수 있으므로 다시 조회한다.
        left = m_liveColliders.find(leftID);
        right = m_liveColliders.find(rightID);
        if (left == m_liveColliders.end() || right == m_liveColliders.end()) return;
        if (event == 0) right->second->OnCollisionEnter(left->second);
        else if (event == 1) right->second->OnCollisionStay(left->second);
        else right->second->OnCollisionExit(left->second);
    }
    bool CollisionManager::Intersect(Collider* left, Collider* right)
    {
        if (!left || !right || left == right || !left->GetOwner() || !right->GetOwner()) return false;
        Transform* leftTransform = left->GetOwner()->GetComponent<Transform>();
        Transform* rightTransform = right->GetOwner()->GetComponent<Transform>();
        if (!leftTransform || !rightTransform) return false;
        // 화면 좌표 대신 월드 좌표를 사용한다. offset은 왼쪽 위 모서리의 보정값이다.
        Vector2 leftPos = leftTransform->GetPosition() + left->GetOffset();
        Vector2 rightPos = rightTransform->GetPosition() + right->GetOffset();
        Vector2 leftSize = left->GetSize() * 100.0f;
        Vector2 rightSize = right->GetSize() * 100.0f;
        if (leftSize.x <= 0 || leftSize.y <= 0 || rightSize.x <= 0 || rightSize.y <= 0) return false;
        const auto leftType = left->GetColliderType();
        const auto rightType = right->GetColliderType();
        if (leftType == enums::ColliderType::Rect2D && rightType == enums::ColliderType::Rect2D)
        {
            // 서로 다른 크기의 상자도 실제 그려지는 모서리로 정확히 검사한다.
            return leftPos.x < rightPos.x + rightSize.x && rightPos.x < leftPos.x + leftSize.x
                && leftPos.y < rightPos.y + rightSize.y && rightPos.y < leftPos.y + leftSize.y;
        }
        if (leftType == enums::ColliderType::Circle2D && rightType == enums::ColliderType::Circle2D)
        {
            // 원본과 동일: size.x * 100은 지름, 왼쪽 위에서 반 크기만큼 이동하면 중심이다.
            Vector2 delta = (leftPos + leftSize / 2.0f) - (rightPos + rightSize / 2.0f);
            const float radiusSum = (leftSize.x + rightSize.x) / 2.0f;
            return delta.x * delta.x + delta.y * delta.y <= radiusSum * radiusSum;
        }
        // 원-사각형은 이 원본 커밋에서 아직 구현하지 않았다.
        return false;
    }
}
