#include "opBallScript.h"
#include "opPaddleScript.h"
#include "opBrickScript.h"
#include <cmath>

#include "../opEngine_SOURCE/opApplication.h"
#include "../opEngine_SOURCE/opGameObject.h"
#include "../opEngine_SOURCE/opInput.h"
#include "../opEngine_SOURCE/opTime.h"
#include "../opEngine_SOURCE/opTransform.h"
#include "../opEngine_SOURCE/opCamera.h"
#include "../opEngine_SOURCE/opRenderer.h"
#include "../opEngine_SOURCE/opCollider.h"

extern op::Application application;

namespace op
{
	BallScript::BallScript() : m_paddle(nullptr), m_velocity(math::Vector2::Zero), m_radius(8.0f), m_isLaunched(false), m_previousPosition(math::Vector2::Zero)
	{

	}

	BallScript::~BallScript()
	{

	}

	void BallScript::Update()
	{
		if (GetForegroundWindow() != application.GetHwnd())
		{
			return;
		}

		GameObject* owner = GetOwner(); // 공 오브젝트

		if (owner == nullptr || m_paddle == nullptr)
		{
			return;
		}

		Transform* ballTransform = owner->GetComponent<Transform>(); // 공에 붙어 있는 위치 정보 가져온다
		if (ballTransform == nullptr)
		{
			return;
		}

		if (Input::GetKeyDown(KeyCode::R)) // R을 처음 누른 순간 공을 막대 위로 되돌린다
		{
			Reset();
			return;
		}

		if (m_isLaunched == false) // 아직 발사하지 않았다면 막대 위에서 대기한다
		{
			FollowPaddle(); // 발사 전에는 움직이는 막대를 따라간다

			if (Input::GetKeyDown(KeyCode::Space)) // Space 누르면
			{
				m_isLaunched = true;

				m_velocity = Vector2(150.0f, -300.0f); // 초당 오른쪽 150픽셀, 위쪽 300픽셀 이동하도록 설정
			}

			return;
		}

		// 여기 부터는 이미 발사된 공만 실행됨

		Vector2 ballPosition = ballTransform->GetPosition(); // 공의 현재 위치
		m_previousPosition = ballPosition;
		ballPosition += m_velocity * Time::DeltaTime();// 위치를 속도 * 델타타임 만큼 이동

		float width = static_cast<float>(application.GetWidth());

		if (ballPosition.x - m_radius <= 0.0f)
		{
			ballPosition.x = m_radius;

			if (m_velocity.x < 0.0f)
			{
				m_velocity.x *= -1;
			}
		}
		else if (ballPosition.x + m_radius >= width)
		{
			ballPosition.x = width - m_radius;

			if (m_velocity.x > 0.0f)
			{
				m_velocity.x *= -1;
			}
		}
		// 위쪽 벽: 공의 위쪽 끝이 화면 경계에 도달했는지 확인
		if (ballPosition.y - m_radius <= 0.0f)
		{
			ballPosition.y = m_radius;

			// 위쪽으로 움직이고 있었다면 아래쪽으로 바꾼다.
			if (m_velocity.y < 0.0f)
			{
				m_velocity.y = -m_velocity.y;
			}
		}


		ballTransform->SetPosition(ballPosition); // 새 위치를 저장
	}

	void BallScript::Render(HDC hdc)
	{
		GameObject* owner = GetOwner(); // 공 오브젝트 가져온다

		if (owner == nullptr || hdc == nullptr)
		{
			return;
		}

		Transform* ballTransform = owner->GetComponent<Transform>();
		if (ballTransform == nullptr)
		{
			return;
		}

		Vector2 ballPosition = ballTransform->GetPosition(); // 공의 게임 좌표를 가져온다

		if (renderer::mainCamera)
		{
			ballPosition = renderer::mainCamera->CalculatePosition(ballPosition);
		}

		HBRUSH brush = CreateSolidBrush(RGB(255, 220, 0)); // 원 내부를 노란색으로 칠할 브러시 만든다
		HBRUSH oldBrush = (HBRUSH)SelectObject(hdc, brush);

		// 중심에서 반지름만큼 떨어진 사각형 안에 원을 그린다.
		Ellipse(
			hdc,
			static_cast<int>(ballPosition.x - m_radius), // 왼쪽 위 x 좌표
			static_cast<int>(ballPosition.y - m_radius),// 왼쪽 위 y 좌표
			static_cast<int>(ballPosition.x + m_radius),// 오른쪽 아래 y 좌표
			static_cast<int>(ballPosition.y + m_radius)// 오른쪽 아래 y 좌표
		);

		SelectObject(hdc, oldBrush);
		DeleteObject(brush);
	}

	// 공을 다시 발사 전 상태로 만드는 함수
	void BallScript::Reset()
	{
		m_isLaunched = false; // 대기 상태로 변경
		m_velocity = math::Vector2::Zero; // 이동 속도 0으로 변경

		FollowPaddle(); // 즉시 막대 위로 이동
	}

	void BallScript::OnCollisionEnter(Collider* other)
	{
		// 발사 전에는 반사하지 않는다
		if (m_isLaunched == false || other == nullptr)
		{
			return;
		}

		GameObject* otherObject = other->GetOwner();

		if (otherObject == nullptr || !otherObject->IsActive())
		{
			return;
		}

		BrickScript* brickScript = otherObject->GetComponent<BrickScript>(); // 상대에게 BrickScript가 붙어 있으면 벽돌이다
		if (brickScript != nullptr)
		{
			ReflectBrick(other);
			return;
		}


		// 부딪힌 상대가 연결된 막대인지 확인한다
		if (m_paddle == nullptr || other->GetOwner() != m_paddle)
		{
			return;
		}

		// 올라가는 공을 아래로 반사하지 않는다
		if (m_velocity.y <= 0.0f)
		{
			return;
		}

		GameObject* owner = GetOwner();
		if (owner == nullptr)
		{
			return;
		}

		Transform* ballTransform = owner->GetComponent<Transform>();
		Transform* paddleTransform = m_paddle->GetComponent<Transform>();

		if (ballTransform == nullptr || paddleTransform == nullptr)
		{
			return;
		}

		Vector2 paddlePosition = paddleTransform->GetPosition();

		// 이동 전 공의 아래쪽 끝이 막대 윗면보다 아래였다면
		// 옆이나 아래에서 부딪힌 것으로 보고 받아치지 않는다
		if (m_previousPosition.y + m_radius > paddlePosition.y + 0.1f)
		{
			return;
		}

		Vector2 ballPosition = ballTransform->GetPosition();

		// 충돌해서 막대 안으로 조금 들어간 공을 막대 바로 위로 꺼내는 코드
		ballPosition.y = paddlePosition.y - m_radius - 0.1f;
		ballTransform->SetPosition(ballPosition);

		// 아래로 내려가던 공을 위로 보낸다
		m_velocity.y = -m_velocity.y;

	}

	void BallScript::OnCollisionStay(Collider* other)
	{
		OnCollisionEnter(other);
	}

	void BallScript::FollowPaddle()
	{
		GameObject* owner = GetOwner(); // 공 오브젝트

		if (owner == nullptr || m_paddle == nullptr)
		{
			return;
		}

		Transform* ballTransform = owner->GetComponent<Transform>(); // ball의 트랜스폼
		Transform* paddleTransform = m_paddle->GetComponent<Transform>(); // paddle의 트랜스폼

		PaddleScript* paddleScript = m_paddle->GetComponent<PaddleScript>(); // paddle의 스크립트

		if (ballTransform == nullptr || paddleTransform == nullptr || paddleScript == nullptr)
		{
			return;
		}

		Vector2 paddlePos = paddleTransform->GetPosition(); // paddle의 위치
		Vector2 paddleSize = paddleScript->GetSize(); // paddle의 사이즈

		// 공의 위치는 중심 좌표를 사용한다
		// 막대 중앙에서, 막대 위쪽에 공을 배치
		Vector2 ballPos(paddlePos.x + paddleSize.x / 2.0f, paddlePos.y - m_radius - 2.0f);

		ballTransform->SetPosition(ballPos);
	}
	void BallScript::ReflectBrick(Collider* other)
	{
		GameObject* owner = GetOwner(); // 공 오브젝트

		if (owner == nullptr || other == nullptr)
		{
			return;
		}

		GameObject* brickObject = other->GetOwner(); // 벽돌 객체

		if (brickObject == nullptr || !brickObject->IsActive())
		{
			return;
		}

		Transform* ballTransform = owner->GetComponent<Transform>(); // 공의 위치 정보
		Transform* brickTransform = brickObject->GetComponent<Transform>(); // 벽돌의 위치 정보
		BrickScript* brickScript = brickObject->GetComponent<BrickScript>(); // 벽돌 스크립트

		if (ballTransform == nullptr || brickTransform == nullptr || brickScript == nullptr)
		{
			return;
		}

		Vector2 ballPosition = ballTransform->GetPosition(); // 공의 중심 좌표

		
		Vector2 brickPosition = brickTransform->GetPosition() + other->GetOffset(); // 벽돌 객체 위치에 콜라이더의 위치 보정값을 더한다
		Vector2 brickSize = other->GetSize() * 100.0f; // 콜라이더 크기에 100을 곱해 실제 크기로 사용
		Vector2 brickCenter = brickPosition + brickSize / 2.0f; // 왼쪽 위에 너비/높이의 절반을 더하면 벽돌 중심이 됨

		// 벽돌 중심을 기준으로 공이 어느쪽에 있는지 계싼
		float deltaX = ballPosition.x - brickCenter.x;
		float deltaY = ballPosition.y - brickCenter.y;

		float overlapX = brickSize.x / 2.0f + m_radius - std::fabs(deltaX);
		float overlapY = brickSize.y / 2.0f + m_radius - std::fabs(deltaY);

		if (overlapX <= 0.0f || overlapY <= 0.0f)
		{
			return;
		}

		// 공을 더 조금 움직여서 꺼낼 수 있는 축을 선택한다.
	//
	// 예: x로는 2픽셀, y로는 15픽셀 이동해야 빠져나온다면
	//     x 방향으로 꺼내고 좌우로 반사한다.
		if (overlapX < overlapY)
		{
			// 공이 벽돌 중심보다 왼쪽에 있다.
			if (deltaX < 0.0f)
			{
				// 공 중심을 벽돌 왼쪽 끝보다 반지름만큼 왼쪽에 놓는다.
				// 0.1은 다시 겹치지 않도록 두는 작은 간격이다.
				ballPosition.x = brickPosition.x - m_radius - 0.1f;

				// 오른쪽으로 벽돌을 향해 움직였다면 왼쪽으로 바꾼다.
				// 이미 왼쪽으로 움직이는 경우에는 다시 뒤집지 않는다.
				if (m_velocity.x > 0.0f)
				{
					m_velocity.x = -m_velocity.x;
				}
			}
			else
			{
				// 공이 벽돌 중심보다 오른쪽에 있다.
				//
				// 벽돌 오른쪽 끝 = brickPosition.x + brickSize.x
				// 그보다 반지름만큼 오른쪽에 공 중심을 놓는다.
				ballPosition.x =
					brickPosition.x + brickSize.x + m_radius + 0.1f;

				// 왼쪽으로 벽돌을 향해 움직였다면 오른쪽으로 바꾼다.
				if (m_velocity.x < 0.0f)
				{
					m_velocity.x = -m_velocity.x;
				}
			}
		}
		else
		{
			// y 방향으로 꺼내는 이동량이 더 작거나 같으면
			// 위아래 방향으로 반사한다.

			if (deltaY < 0.0f)
			{
				// 공이 벽돌 중심보다 위쪽에 있다.
				// 공을 벽돌 윗면 바로 위로 꺼낸다.
				ballPosition.y = brickPosition.y - m_radius - 0.1f;

				// 아래로 내려가던 공을 위로 보낸다.
				if (m_velocity.y > 0.0f)
				{
					m_velocity.y = -m_velocity.y;
				}
			}
			else
			{
				// 공이 벽돌 중심보다 아래쪽에 있다.
				//
				// 벽돌 아래쪽 끝 = brickPosition.y + brickSize.y
				// 그보다 반지름만큼 아래에 공 중심을 놓는다.
				ballPosition.y =
					brickPosition.y + brickSize.y + m_radius + 0.1f;

				// 위로 올라가던 공을 아래로 보낸다.
				if (m_velocity.y < 0.0f)
				{
					m_velocity.y = -m_velocity.y;
				}
			}
		}

		// 위에서 계산한 위치는 지역 변수다.
		// Transform에 저장해야 실제 공 위치가 변경된다.
		ballTransform->SetPosition(ballPosition);

		// 반사를 처리했으므로 이제 벽돌을 부순다.
		// Break 안에서 삭제를 예약하고,
		// 엔진이 프레임 마지막에 실제로 삭제한다.
		brickScript->Break();

	}
}