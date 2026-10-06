#include "opFloorScript.h"

#include "..\\opEngine_SOURCE\\opAudioSource.h"
#include "..\\opEngine_SOURCE\\opCollider.h"
#include "..\\opEngine_SOURCE\\opGameObject.h"
#include "..\\opEngine_SOURCE\\opTransform.h"
#include "..\\opEngine_SOURCE\\opRigidbody.h"

namespace op
{
	FloorScript::FloorScript()
	{
	}

	FloorScript::~FloorScript()
	{
	}

	void FloorScript::Initialize()
	{
	}

	void FloorScript::Update()
	{
	}

	void FloorScript::LateUpdate()
	{
	}

	void FloorScript::Render(HDC hdc)
	{
	}

	void FloorScript::OnCollisionEnter(Collider* other)
	{
		if (!other || !other->GetOwner() || !GetOwner())
		{
			return;
		}

		GameObject* player = other->GetOwner();

		Rigidbody* playerRb = player->GetComponent<Rigidbody>();
		Transform* playerTr = player->GetComponent<Transform>();

		Transform* floorTr = GetOwner()->GetComponent<Transform>();
		Collider* floorCol = GetOwner()->GetComponent<Collider>();

		if (!playerRb || !playerTr || !floorTr || !floorCol)
		{
			return;
		}

		Vector2 velocity = playerRb->GetVelocity();

		// 上昇中には着地させない
		// 점프해서 올라가는 중에는 다시 바닥 상태로 만들지 않는다.
		if (velocity.y < 0.0f)
		{
			return;
		}

		Vector2 playerPos = playerTr->GetPosition();
		Vector2 playerOffset = other->GetOffset();
		Vector2 playerSize = other->GetSize() * 100.0f;

		Vector2 floorPos =
			floorTr->GetPosition() + floorCol->GetOffset();

		const float playerTop = playerPos.y + playerOffset.y;
		const float playerBottom = playerTop + playerSize.y;
		const float floorTop = floorPos.y;

		// 바닥 윗면을 걸쳐 있는 경우에 착지 처리
		if (playerTop >= floorTop || playerBottom < floorTop)
		{
			return;
		}

		// 충돌체의 아래쪽을 바닥 윗면에 맞춘다.
		// 현재 사각형 판정은 정확히 닿으면 충돌이 아니므로
		// 1픽셀 겹침을 유지하여 접촉 상태가 계속되게 한다.
		playerPos.y =
			floorTop - playerOffset.y - playerSize.y + 1.0f;

		playerTr->SetPosition(playerPos);

		velocity.y = 0.0f;
		playerRb->SetVelocity(velocity);

		// 공중 상태에서 바닥에 착지했을 때만 재생 시작
		if (!playerRb->IsGround())
		{
			AudioSource* audioSource =
				GetOwner()->GetComponent<AudioSource>();

			if (audioSource)
			{
				audioSource->SetLoop(true);
				audioSource->Play();
			}
		}

		playerRb->SetGround(true);
	}

	void FloorScript::OnCollisionStay(Collider* other)
	{
		// 계속 접촉하는 동안에도 착지 상태와 위치를 확인한다.
		OnCollisionEnter(other);
	}

	void FloorScript::OnCollisionExit(Collider* other)
	{
		if (!other || !other->GetOwner())
		{
			return;
		}

		Rigidbody* playerRb =
			other->GetOwner()->GetComponent<Rigidbody>();

		if (playerRb)
		{
			// 바닥을 벗어나면 다시 중력을 적용한다.
			playerRb->SetGround(false);
		}
	}
}