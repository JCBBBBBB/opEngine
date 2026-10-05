# Collision implementation

Original: eazuooz/YamYam_Engine

1. 70f27c4 - BoxCollider 구현
   - op Collider/BoxCollider2D/CircleCollider2D, player and Animal sample.
   - Keeps existing WASD controls and resource paths.
   - Fixes owner initialization, idle Cat null access, client buffer dimensions,
     GDI object ownership, folder frame ordering/resource keys and one-shot events.
   - Collider outlines use the same camera conversion as sprites.

Validation: build the solution, then run Tests/RunTests.ps1.
