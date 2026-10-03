#include "opCamera.h"
#include "opGameObject.h"
#include "opTransform.h"
#include "opApplication.h"

extern op::Application application;


namespace op
{
	Camera::Camera() : Component(enums::ComponentType::Camera), m_distance(Vector2::Zero), m_resolution(Vector2::Zero), m_lookPosition(Vector2::Zero), m_target(nullptr)
	{
	}

	Camera::~Camera()
	{
	}

	void Camera::Initialize()
	{
		m_resolution.x = application.GetWidth();
		m_resolution.y = application.GetHeight();
	}

	void Camera::Update()
	{
		if (m_target)
		{
			Transform* transform = m_target->GetComponent<Transform>();
			m_lookPosition = transform->GetPosition();
		}

		Transform* cameraTransform = GetOwner()->GetComponent<Transform>();
		m_lookPosition = cameraTransform->GetPosition();

		m_distance = m_lookPosition - (m_resolution / 2.0f);
	}

	void Camera::LateUpdate()
	{
	}

	void Camera::Render(HDC hdc)
	{
	}
}