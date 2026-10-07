#include "opBallScript.h"
#include "opPaddleScript.h"

#include "../opEngine_SOURCE/opApplication.h"
#include "../opEngine_SOURCE/opGameObject.h"
#include "../opEngine_SOURCE/opInput.h"
#include "../opEngine_SOURCE/opTime.h"
#include "../opEngine_SOURCE/opTransform.h"
#include "../opEngine_SOURCE/opCamera.h"
#include "../opEngine_SOURCE/opRenderer.h"

extern op::Application application;

namespace op
{
	BallScript::BallScript() : m_paddle(nullptr), m_velocity(math::Vector2::Zero), m_radius(8.0f), m_isLaunched(false)
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
}