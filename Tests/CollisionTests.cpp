#include "opApplication.h"
#include "opBoxCollider2D.h"
#include "opTransform.h"
#include "opRenderer.h"
#include <stdexcept>
#include "../opEngine_Window/opLoadResources.h"
#include "../opEngine_Window/opLoadScenes.h"
#include "opTime.h"
#include "opAnimator.h"

op::Application application;
void Require(bool condition, const char* message)
{
    if (!condition) throw std::runtime_error(message);
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
        SceneManager::LateUpdate();
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
