#include "opBrickScript.h"
#include "opPlayScene.h"

#include "../opEngine_SOURCE/opGameObject.h"
#include "../opEngine_SOURCE/opObject.h"
#include "../opEngine_SOURCE/opTransform.h"
#include "../opEngine_SOURCE/opCamera.h"
#include "../opEngine_SOURCE/opRenderer.h"

namespace op
{
	BrickScript::BrickScript() : m_size(80.0f, 24.0f), m_isBroken(false), m_playScene(nullptr)
	{
	}

	BrickScript::~BrickScript()
	{
	}

	void BrickScript::Render(HDC hdc)
	{
		GameObject* owner = GetOwner();

		if (owner == nullptr || hdc == nullptr || m_isBroken)
		{
			return;
		}

		Transform* brickTransform = owner->GetComponent<Transform>();
		if (brickTransform == nullptr)
		{
			return;
		}

		Vector2 brickPosition = brickTransform->GetPosition();
		if (renderer::mainCamera)
		{
			brickPosition = renderer::mainCamera->CalculatePosition(brickPosition);
		}

		RECT rect = {};
		rect.left = static_cast<LONG>(brickPosition.x);
		rect.top = static_cast<LONG>(brickPosition.y);
		rect.right = static_cast<LONG>(brickPosition.x + m_size.x);
		rect.bottom = static_cast<LONG>(brickPosition.y + m_size.y);

		HBRUSH brush = CreateSolidBrush(RGB(255, 120, 100));

		FillRect(hdc, &rect, brush);

		DeleteObject(brush);
	}

	void BrickScript::Break()
	{
		GameObject* owner = GetOwner();

		// 같은 벽돌을 여러 번 부수지 않는다.
		if (owner == nullptr || m_isBroken || owner->IsDead())
		{
			return;
		}

		m_isBroken = true;

		// 연결된 PlayScene에 10점을 추가한다
		if (m_playScene != nullptr)
		{
			m_playScene->OnBrickBroken(); // 점수를 올리고 남은 벽돌 개수를 줄인다
		}

		// 즉시 delete하지 않고 Dead 상태로 만든다
		// 실제 삭제는 엔진의 프레임 마지막 Destroy에서 처리한다
		object::Destroy(owner);
	}
}