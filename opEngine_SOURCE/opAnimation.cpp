#include "opAnimation.h"
#include "opTime.h"
#include "opTransform.h"
#include "opGameObject.h"
#include "opAnimator.h"
#include "opRenderer.h"
#include "opCamera.h"

namespace op
{
	Animation::Animation() : Resource(enums::ResourceType::Animation), m_animator(nullptr), m_texture(nullptr), m_animationSheet{}, m_index(-1), m_time(0.0f), m_isComplete(false)
	{

	}

	Animation::~Animation()
	{

	}

	HRESULT Animation::Load(const std::wstring& path)
	{
		return E_NOTIMPL;
	}

	void Animation::Update()
	{
		if (m_isComplete)
		{
			return;
		}

		m_time += Time::DeltaTime();

		if (m_animationSheet[m_index].duration < m_time)
		{
			m_time = 0.0f;
			if (m_index < m_animationSheet.size() - 1)
			{
				m_index++;
			}
			else
			{
				m_isComplete = true;
			}
		}
	}

	void Animation::Render(HDC hdc)
	{
		if (m_texture == nullptr)
		{
			return;
		}

		GameObject* gameObject = m_animator->GetOwner();
		Transform* transform = gameObject->GetComponent<Transform>();
		Vector2 pos = transform->GetPosition();

		if (renderer::mainCamera)
		{
			pos = renderer::mainCamera->CalculatePosition(pos);
		}

		BLENDFUNCTION func = {};
		func.BlendOp = AC_SRC_OVER;
		func.BlendFlags = 0;
		func.AlphaFormat = AC_SRC_ALPHA;
		func.SourceConstantAlpha = 125; // 0(transparent) ~ 255(Opaque)

		Sprite sprite = m_animationSheet[m_index];
		HDC imgHdc = m_texture->GetHdc();

		AlphaBlend(hdc, pos.x, pos.y, sprite.size.x * 5, sprite.size.y * 5, imgHdc, sprite.leftTop.x, sprite.leftTop.y, sprite.size.x, sprite.size.y, func);
	}

	// 원본 이미지에서 어느 부분을 표시할지 정보 저장
	void Animation::CreateAnimation(const std::wstring& name, graphics::Texture* spriteSheet, Vector2 leftTop, Vector2 size, Vector2 offset, UINT spriteLength, float duration)
	{
		m_texture = spriteSheet;

		for (size_t i = 0; i < spriteLength; i++)
		{
			Sprite sprite = {};
			sprite.leftTop.x = leftTop.x + (size.x * i);
			sprite.leftTop.y = leftTop.y;
			sprite.size = size;
			sprite.offset = offset;
			sprite.duration = duration;

			m_animationSheet.push_back(sprite);
		}
	}

	void Animation::Reset()
	{
		m_time = 0.0f;
		m_index = 0;
		m_isComplete = false;
	}
}