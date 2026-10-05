# Collision implementation

Original: eazuooz/YamYam_Engine

1. 70f27c4 - BoxCollider 구현
   - op Collider/BoxCollider2D/CircleCollider2D, player and Animal sample.
   - Keeps existing WASD controls and resource paths.
   - Fixes owner initialization, idle Cat null access, client buffer dimensions,
     GDI object ownership, folder frame ordering/resource keys and one-shot events.
   - Collider outlines use the same camera conversion as sprites.

Validation: build the solution, then run Tests/RunTests.ps1.

2. 099e767 - Collision Manager 충돌체 레이어 마스킹 체크 구조 및 충돌 체크 로직
   - Layer mask and collider pair traversal. Geometry/events follow in step 3.
   - Checks after object updates, skips self/duplicates, snapshots object lists.
