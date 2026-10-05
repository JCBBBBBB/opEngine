#pragma once
#include "CommonInclude.h"
#include "opCollider.h"
namespace op
{
    class Scene;
    class CollisionManager
    {
    public:
        static void Initialize();
        static void Update();
        static void LateUpdate();
        static void Render(HDC hdc);
        static void CollisionLayerCheck(enums::LayerType left, enums::LayerType right, bool enable);
        static bool IsLayerEnabled(enums::LayerType left, enums::LayerType right);
        static void LayerCollision(Scene* scene, enums::LayerType left, enums::LayerType right);
        static void ColliderCollision(Collider* left, Collider* right);
        static bool Intersect(Collider* left, Collider* right);
        static void Reset();
        static void RegisterCollider(Collider* collider);
        static void ForgetCollider(UINT32 id);
        static size_t GetContactCount() { return m_collisionMap.size(); }
    private:
        static UINT64 PairID(UINT32 left, UINT32 right);
        static void Dispatch(UINT64 pair, int event);
        static std::unordered_map<UINT64, bool> m_collisionMap;
        static std::unordered_map<UINT32, Collider*> m_liveColliders;
        static std::unordered_set<UINT64> m_checkedPairs;
        static Scene* m_collisionScene;
        static constexpr size_t LayerCount = static_cast<size_t>(enums::LayerType::Max);
        static std::bitset<LayerCount> m_collisionLayerMatrix[LayerCount];
    };
}
