#include "opSpriteRenderer.h"
#include "opGameObject.h"
#include "opTransform.h"

namespace op
{
	SpriteRenderer::SpriteRenderer()
	{

	}

	SpriteRenderer::~SpriteRenderer()
	{

	}

	void SpriteRenderer::Initialize()
	{

	}

	void SpriteRenderer::Update()
	{

	}

	void SpriteRenderer::LateUpdate()
	{

	}

	void SpriteRenderer::Render(HDC hdc)
	{
		//파랑 브러쉬 생성
		HBRUSH blueBrush
			= CreateSolidBrush(RGB(255, 0, 255));

		// 파랑 브러쉬 DC에 선택 그리고 흰색 브러쉬 반환값 반환
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, blueBrush);

		HPEN redPen = CreatePen(PS_SOLID, 2, RGB(rand() % 255, rand() % 255, rand() % 255));
		HPEN oldPen = (HPEN)SelectObject(hdc, redPen);
		SelectObject(hdc, oldPen);

		Transform* tr = GetOwner()->GetComponent<Transform>();
		Rectangle(hdc, tr->GetPosX(), tr->GetPosY()
			, 100 + tr->GetPosX(), 100 + tr->GetPosY());

		SelectObject(hdc, oldBrush);
		DeleteObject(blueBrush);
		DeleteObject(redPen);
	}
}