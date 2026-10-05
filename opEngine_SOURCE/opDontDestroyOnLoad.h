#pragma once
#include "opScene.h"

namespace op
{
    // 일반 씬을 전환해도 계속 실행할 오브젝트를 보관하는 씬.
    class DontDestroyOnLoad : public Scene
    {
    public:
        DontDestroyOnLoad();
        ~DontDestroyOnLoad() override;
        void Initialize() override;
        void Update() override;
        void LateUpdate() override;
        void Render(HDC hdc) override;
        void OnEnter() override;
        void OnExit() override;
    };
}
