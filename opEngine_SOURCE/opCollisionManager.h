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
    private:
        static constexpr size_t LayerCount = static_cast<size_t>(enums::LayerType::Max);
        static std::bitset<LayerCount> m_collisionLayerMatrix[LayerCount];
    };
}
