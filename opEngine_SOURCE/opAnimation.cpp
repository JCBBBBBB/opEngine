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

	// 처음 상태는 m_index = 0, m_time = 0, m_isComplete = false
	void Animation::Update()
	{
		if (m_isComplete)
		{
			return; // 이미 재생 끝났으면 더 진행하지 않음
		}

		m_time += Time::DeltaTime(); // 지난 업데이트부터 흐른 시간을 현재 프레임 시간에 더함

		// 현재 프레임의 유지 시간을 넘었는지 확인
		// 지금은 0.5초를 넘으면 다음 처리로 돌아간다
		if (m_animationSheet[m_index].duration < m_time)
		{
			m_time = 0.0f; // 다음 프레임의 시간을 새로 측정하기 위해 0으로 설정
	
			if (m_index < m_animationSheet.size() - 1) // 현재 프레임이 마지막 프레임보다 앞이라면
			{
				m_index++; // 다음 프레임 선택
			}
			else
			{
				m_isComplete = true; // 현재
			}
		}
	}

	// 현재 프레임의 그림을 가져와 플레이어 위치에 그리는 함수
    void Animation::Render(HDC hdc)
    {
        if (m_texture == nullptr)
            return;

        GameObject* gameObject = m_animator->GetOwner();
        Transform* transform =
            gameObject->GetComponent<Transform>();

        Vector2 pos = transform->GetPosition();
        float rot = transform->GetRotation();
        Vector2 scale = transform->GetScale();

        if (renderer::mainCamera)
        {
            pos = renderer::mainCamera->CalculatePosition(pos);
        }

        Sprite sprite = m_animationSheet[m_index];

        graphics::Texture::TextureType type =
            m_texture->GetTextureType();

        if (type == graphics::Texture::TextureType::Bmp)
        {
            HDC imgHdc = m_texture->GetHdc();

            if (m_texture->IsAlpha())
            {
                BLENDFUNCTION func = {};
                func.BlendOp = AC_SRC_OVER;
                func.BlendFlags = 0;
                func.AlphaFormat = AC_SRC_ALPHA;
                func.SourceConstantAlpha = 255;

                AlphaBlend(
                    hdc,
                    pos.x - (sprite.size.x / 2.0f) + sprite.offset.x,
                    pos.y - (sprite.size.y / 2.0f) + sprite.offset.y,
                    sprite.size.x * scale.x,
                    sprite.size.y * scale.y,
                    imgHdc,
                    sprite.leftTop.x,
                    sprite.leftTop.y,
                    sprite.size.x,
                    sprite.size.y,
                    func);
            }
            else
            {
                TransparentBlt(
                    hdc,
                    pos.x - (sprite.size.x / 2.0f) + sprite.offset.x,
                    pos.y - (sprite.size.y / 2.0f) + sprite.offset.y,
                    sprite.size.x * scale.x,
                    sprite.size.y * scale.y,
                    imgHdc,
                    sprite.leftTop.x,
                    sprite.leftTop.y,
                    sprite.size.x,
                    sprite.size.y,
                    RGB(255, 0, 255));
            }
        }
        else if (type == graphics::Texture::TextureType::Png)
        {
            Gdiplus::ImageAttributes imgAtt = {};

            imgAtt.SetColorKey(
                Gdiplus::Color(230, 230, 230),
                Gdiplus::Color(255, 255, 255));

            Gdiplus::Graphics graphics(hdc);

            graphics.TranslateTransform(pos.x, pos.y);
            graphics.RotateTransform(rot);
            graphics.TranslateTransform(-pos.x, -pos.y);

            graphics.DrawImage(
                m_texture->GetImage(),
                Gdiplus::Rect(
                    pos.x - (sprite.size.x / 2.0f),
                    pos.y - (sprite.size.y / 2.0f),
                    sprite.size.x * scale.x,
                    sprite.size.y * scale.y),
                sprite.leftTop.x,
                sprite.leftTop.y,
                sprite.size.x,
                sprite.size.y,
                Gdiplus::UnitPixel,
                /*&imgAtt*/nullptr);
        }
    }

	// 프레임 정보 구성
	void Animation::CreateAnimation(const std::wstring& name, graphics::Texture* spriteSheet, Vector2 leftTop, Vector2 size, Vector2 offset, UINT spriteLength, float duration)
	{
		// sprite에는 그림 데이터가 들어 있지 않다. 원본 이미지의 어느 영역을 보여줄지에 대한 정보만 들어 있다

		m_texture = spriteSheet; // 원본 Texture의 주소 저장

		for (size_t i = 0; i < spriteLength; i++) // 현재 씬에서는 spriteLength가 4이므로 4번 반복
		{
			Sprite sprite = {}; // 프레임 한 장의 정보를 담을 변수
			sprite.leftTop.x = leftTop.x + (size.x * i); // 가로 방향으로 프레임 너비만큼 이동한 원본 좌표 계산.
			sprite.leftTop.y = leftTop.y;
			sprite.size = size; // 각 프레임은 32x32
			sprite.offset = offset; // 현재는 (0,0)
			sprite.duration = duration; // 각 프레임 유지 시간 0.5초

			m_animationSheet.push_back(sprite); // 완성한 프레임 정보를 목록에 추가
		}
	}

	void Animation::Reset()
	{
		m_time = 0.0f; // 현재 프레임의 누적 시간을 0으로
		m_index = 0; // 첫 프레임 선택
		m_isComplete = false; // 아직 재생이 끝나지 않은 상태
	}
}