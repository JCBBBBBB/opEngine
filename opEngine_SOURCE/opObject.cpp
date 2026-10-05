#include "opObject.h"

namespace op::object
{
    void DontDestroyOnLoad(GameObject* gameObject)
    {
        Scene* persistent = SceneManager::GetDontDestroyOnLoad();
        if (!gameObject || gameObject->IsDead() || !persistent) return;
        Scene* source = gameObject->GetScene();
        if (!source || source == persistent) return;
        const auto layerType = gameObject->GetLayerType();
        if (!persistent->GetLayer(layerType)) return;
        source->EraseGameObject(gameObject);
        persistent->AddGameObject(gameObject, layerType);
    }
}
