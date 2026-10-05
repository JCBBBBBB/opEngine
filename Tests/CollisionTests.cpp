#include "opApplication.h"
#include "opBoxCollider2D.h"
#include "opTransform.h"
#include "opRenderer.h"
#include <stdexcept>
#include "../opEngine_Window/opLoadResources.h"
#include "../opEngine_Window/opLoadScenes.h"
#include "opTime.h"
#include "opAnimator.h"
#include "opCollisionManager.h"
#include "opScript.h"
#include "../opEngine_Window/opPlayerScript.h"

op::Application application;
void Require(bool condition, const char* message)
{
    if (!condition) throw std::runtime_error(message);
}

class CountingScript : public op::Script
{
public:
    int enters = 0, stays = 0, exits = 0;
    bool initializedWithOwner = false;
    std::function<void(op::Collider*)> entered;
    void Initialize() override { initializedWithOwner = GetOwner() != nullptr; }
    void OnCollisionEnter(op::Collider* other) override { ++enters; if (entered) entered(other); }
    void OnCollisionStay(op::Collider*) override { ++stays; }
    void OnCollisionExit(op::Collider*) override { ++exits; }
};

op::GameObject* TestObject(op::Scene* scene, op::enums::LayerType layer, op::math::Vector2 position)
{
    auto* object = new op::GameObject();
    object->AddComponent<op::BoxCollider2D>();
    object->AddComponent<CountingScript>();
    object->GetComponent<op::Transform>()->SetPosition(position);
    scene->AddGameObject(object, layer);
    return object;
}

void RectangleTests()
{
    using namespace op;
    CollisionManager::Initialize();
    // 원본의 중심/왼쪽 위 혼동을 드러내는 서로 다른 크기의 상자.
    {
        GameObject left, right;
        auto* a = left.AddComponent<BoxCollider2D>();
        auto* b = right.AddComponent<BoxCollider2D>();
        Require(a->GetID() != b->GetID(), "Unique collider IDs");
        a->SetSize(Vector2(2.0f, 1.0f));
        right.GetComponent<Transform>()->SetPosition(Vector2(190.0f, 10.0f));
        Require(CollisionManager::Intersect(a, b), "Unequal-size boxes overlap");
        Require(CollisionManager::Intersect(b, a), "Symmetric rectangle intersection");
        right.GetComponent<Transform>()->SetPosition(Vector2(200.0f, 10.0f));
        Require(!CollisionManager::Intersect(a, b), "Rectangle edge-only contact excluded");
        b->SetOffset(Vector2(-20.0f, 0.0f));
        Require(CollisionManager::Intersect(a, b), "Rectangle offset");
        a->SetSize(Vector2::Zero);
        Require(!CollisionManager::Intersect(a, b), "Empty rectangle excluded");
        a->SetSize(Vector2::One);
        right.GetComponent<Transform>()->SetPosition(Vector2(30.0f, 0.0f));
        CollisionManager::ColliderCollision(a, b); // Script가 없어도 안전하다.
    }
    Require(CollisionManager::GetContactCount() == 0, "Destructor contact cleanup");
    auto* scene = SceneManager::CreateScene<Scene>(L"CollisionTests");
    CollisionManager::CollisionLayerCheck(enums::LayerType::Player, enums::LayerType::Animal, true);
    auto* left = TestObject(scene, enums::LayerType::Player, Vector2::Zero);
    auto* right = TestObject(scene, enums::LayerType::Animal, Vector2(50.0f, 0.0f));
    auto* a = left->GetComponent<CountingScript>();
    auto* b = right->GetComponent<CountingScript>();
    Require(a->initializedWithOwner, "Owner set before Initialize");
    CollisionManager::Update();
    Require(a->enters == 1 && b->enters == 1, "Enter once on both objects");
    CollisionManager::Update();
    Require(a->stays == 1 && b->stays == 1, "Stay once per frame");
    right->GetComponent<Transform>()->SetPosition(Vector2(500.0f, 0.0f));
    CollisionManager::Update();
    Require(a->exits == 1 && b->exits == 1, "Exit on separation");
    right->GetComponent<Transform>()->SetPosition(Vector2(50.0f, 0.0f));
    CollisionManager::Update();
    Require(a->enters == 2 && b->enters == 2, "Re-enter after separation");
    CollisionManager::ColliderCollision(right->GetComponent<Collider>(), left->GetComponent<Collider>());
    Require(a->enters == 2 && a->stays == 2, "Canonical reversed pair ID");
    CollisionManager::CollisionLayerCheck(enums::LayerType::Animal, enums::LayerType::Player, false);
    CollisionManager::Update();
    Require(a->exits == 2 && b->exits == 2, "Exit when mask disabled");
    CollisionManager::CollisionLayerCheck(enums::LayerType::Animal, enums::LayerType::Player, true);
    CollisionManager::Update();
    right->SetActive(false);
    CollisionManager::Update();
    Require(a->exits == 3 && b->exits == 3, "Exit when paused");
    right->SetActive(true);
    CollisionManager::Update();
    Require(a->enters == 4 && b->enters == 4, "Enter after reactivation");
    object::Destroy(right);
    SceneManager::Destroy();
    Require(CollisionManager::GetContactCount() == 0, "Deleted collider forgotten");
    CollisionManager::Update();
    auto* noScript = new GameObject();
    noScript->AddComponent<BoxCollider2D>();
    scene->AddGameObject(noScript, enums::LayerType::Animal);
    CollisionManager::Update();
    object::Destroy(noScript);
    CollisionManager::Update();
    Require(a->exits == 4, "Exit before deferred deletion");
    SceneManager::Destroy();

    // 같은 레이어의 3개는 3쌍, 각 오브젝트는 2회씩 Enter를 받는다.
    scene = SceneManager::CreateScene<Scene>(L"SameLayerTests");
    CollisionManager::CollisionLayerCheck(enums::LayerType::Player, enums::LayerType::Player, true);
    auto* one = TestObject(scene, enums::LayerType::Player, Vector2::Zero);
    auto* two = TestObject(scene, enums::LayerType::Player, Vector2::Zero);
    auto* three = TestObject(scene, enums::LayerType::Player, Vector2::Zero);
    CollisionManager::Update();
    Require(CollisionManager::GetContactCount() == 3, "Same-layer unique pairs");
    for (auto* object : {one, two, three})
        Require(object->GetComponent<CountingScript>()->enters == 2, "No self/duplicate events");
    auto* target = SceneManager::CreateScene<Scene>(L"TransitionTests");
    SceneManager::LoadScene(L"SameLayerTests");
    CollisionManager::Update();
    Require(one->GetComponent<CountingScript>()->enters == 4, "Fresh contacts on scene enter");
    Require(SceneManager::LoadScene(L"MissingScene") == nullptr, "Missing scene lookup");
    Require(SceneManager::GetActiveScene() == scene && CollisionManager::GetContactCount() == 3, "Invalid load preserves current contacts");
    SceneManager::LoadScene(L"TransitionTests");
    Require(CollisionManager::GetContactCount() == 0, "Scene switch clears contacts");

    // 콜백에서 같은 레이어에 새 오브젝트를 추가해도 검사 목록은 안전하다.
    scene = SceneManager::CreateScene<Scene>(L"CallbackTests");
    one = TestObject(scene, enums::LayerType::Player, Vector2::Zero);
    two = TestObject(scene, enums::LayerType::Player, Vector2::Zero);
    bool spawned = false;
    one->GetComponent<CountingScript>()->entered = [scene, &spawned](Collider*)
    {
        if (!spawned) { spawned = true; TestObject(scene, enums::LayerType::Player, Vector2::Zero); }
    };
    CollisionManager::Update();
    Require(spawned && CollisionManager::GetContactCount() == 1, "Callback spawn snapshot");
    CollisionManager::Update();
    Require(CollisionManager::GetContactCount() == 3, "Spawned collider checked next frame");

    // 원본 PlayerScript의 충돌 데모 동작 확인.
    scene = SceneManager::CreateScene<Scene>(L"PlayerDemoTests");
    one = TestObject(scene, enums::LayerType::Player, Vector2::Zero);
    one->AddComponent<PlayerScript>();
    two = TestObject(scene, enums::LayerType::Animal, Vector2::Zero);
    CollisionManager::Update();
    Vector2 moved = two->GetComponent<Transform>()->GetPosition();
    Require(moved.x == 400.0f && moved.y == 500.0f, "Original collision demo teleport");

    scene = SceneManager::CreateScene<Scene>(L"SwitchCallbackTests");
    one = TestObject(scene, enums::LayerType::Player, Vector2::Zero);
    two = TestObject(scene, enums::LayerType::Player, Vector2::Zero);
    one->GetComponent<CountingScript>()->entered = [](Collider*) { SceneManager::LoadScene(L"TransitionTests"); };
    CollisionManager::Update();
    Require(SceneManager::GetActiveScene() == target && CollisionManager::GetContactCount() == 0, "Scene change in callback");
    SceneManager::Release();
    std::cout << "Rectangle/lifecycle/callback regression tests passed\n";
}

void IntegrationTests()
{
    using namespace op;
    ULONG_PTR token = 0;
    Gdiplus::GdiplusStartupInput startup;
    Require(Gdiplus::GdiplusStartup(&token, &startup, nullptr) == Gdiplus::Ok, "GDI+ startup");
    HWND window = CreateWindowW(L"STATIC", L"Engine test", WS_OVERLAPPEDWINDOW,
        0, 0, 672, 846, nullptr, nullptr, GetModuleHandleW(nullptr), nullptr);
    Require(window != nullptr, "Test window");
    application.Initialize(window, 672, 846);
    RECT client{};
    GetClientRect(window, &client);
    Require(client.right == 672 && client.bottom == 846, "Client buffer dimensions");
    LoadResources();
    Require(Resources::Find<graphics::Texture>(L"Player") != nullptr, "Player image load");
    LoadScenes();
    for (int i = 0; i < 20; ++i)
    {
        Time::Update();
        SceneManager::Update();
        application.LateUpdate();
        application.Render();
    }
    // N키와 동일한 씬 전환 경로를 여러 번 확인한다.
    for (int i = 0; i < 10; ++i)
    {
        Require(SceneManager::LoadScene(L"TitleScene") != nullptr, "Title transition");
        SceneManager::Update();
        application.Render();
        Require(SceneManager::LoadScene(L"PlayScene") != nullptr, "Play transition");
        SceneManager::Update();
        application.Render();
    }
    GameObject animated;
    auto* animator = animated.AddComponent<Animator>();
    animator->CreateAnimation(L"Once", Resources::Find<graphics::Texture>(L"Player"),
        Vector2::Zero, Vector2(250.0f, 250.0f), Vector2::Zero, 1, 0.00001f);
    int completed = 0;
    animator->GetCompleteEvent(L"Once") = [&completed]() { ++completed; };
    animator->PlayAnimation(L"Once", false);
    Sleep(2);
    Time::Update();
    for (int i = 0; i < 20; ++i) animator->Update();
    Require(completed == 1, "One-shot completion event fires once");
    application.Release();
    renderer::mainCamera = nullptr;
    DestroyWindow(window);
    Gdiplus::GdiplusShutdown(token);
    std::cout << "Real scene/resource/transition tests passed\n";
}

int RunTests()
{
    using namespace op;
    CollisionManager::Initialize();
    CollisionManager::Update(); // 씬이 없어도 안전해야 한다.
    CollisionManager::CollisionLayerCheck(enums::LayerType::Animal, enums::LayerType::Player, true);
    Require(CollisionManager::IsLayerEnabled(enums::LayerType::Player, enums::LayerType::Animal), "Symmetric layer mask");
    CollisionManager::CollisionLayerCheck(enums::LayerType::Player, enums::LayerType::Animal, false);
    Require(!CollisionManager::IsLayerEnabled(enums::LayerType::Animal, enums::LayerType::Player), "Disabled layer mask");
    CollisionManager::CollisionLayerCheck(enums::LayerType::Max, enums::LayerType::Player, true);
    Require(!CollisionManager::IsLayerEnabled(enums::LayerType::Max, enums::LayerType::Player), "Layer index bounds");
    GameObject object;
    auto* box = object.AddComponent<BoxCollider2D>();
    Require(box->GetOwner() == &object, "Collider owner");
    Require(object.GetComponent<Collider>() == box, "Collider lookup");
    box->SetOffset(Vector2(-50.0f, -50.0f));
    Require(box->GetOffset().x == -50.0f, "Offset");
    HDC screen = GetDC(nullptr);
    HDC dc = CreateCompatibleDC(screen);
    HBITMAP bitmap = CreateCompatibleBitmap(screen, 256, 256);
    HGDIOBJ previous = SelectObject(dc, bitmap);
    object.GetComponent<Transform>()->SetPosition(Vector2(100.0f, 100.0f));
    renderer::mainCamera = nullptr;
    for (int i = 0; i < 100; ++i) box->Render(dc);
    GdiFlush();
    DWORD handles = GetGuiResources(GetCurrentProcess(), GR_GDIOBJECTS);
    for (int i = 0; i < 1000; ++i) box->Render(dc);
    GdiFlush();
    Require(GetGuiResources(GetCurrentProcess(), GR_GDIOBJECTS) == handles, "GDI handle leak");
    Require(GetPixel(dc, 50, 75) == RGB(0, 255, 0), "Box outline position");
    SelectObject(dc, previous);
    DeleteObject(bitmap);
    DeleteDC(dc);
    ReleaseDC(nullptr, screen);
    RectangleTests();
    IntegrationTests();
    std::cout << "Collider rendering tests passed\n";
    return 0;
}

int main()
{
    SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
    try { return RunTests(); }
    catch (const std::exception& error) { std::cerr << error.what() << "\n"; return 1; }
}
