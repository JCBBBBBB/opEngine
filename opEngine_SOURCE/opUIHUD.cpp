#include "opUIHUD.h"
#include "opResources.h"

namespace op
{
	UIHUD::UIHUD()
		: UIBase(enums::UIType::HpBar)
		, m_texture(nullptr)
	{
	}

	UIHUD::~UIHUD()
	{
	}

	void UIHUD::OnInit()
	{
		m_texture = Resources::Find<graphics::Texture>(L"HPBAR");
	}

	void UIHUD::OnActive()
	{
	}

	void UIHUD::OnInActive()
	{
	}

	void UIHUD::OnUpdate()
	{
	}

	void UIHUD::OnLateUpdate()
	{
	}

	void UIHUD::OnRender(HDC hdc)
	{
		// 이미지 로딩 실패 시 잘못된 주소에 접근하지 않도록 확인
		if (!m_texture || !m_texture->GetHdc())
		{
			return;
		}

		int width = static_cast<int>(m_texture->GetWidth());
		int height = static_cast<int>(m_texture->GetHeight());

		TransparentBlt(
			hdc,
			0, 0,
			width, height,
			m_texture->GetHdc(),
			0, 0,
			width / 2, height,
			RGB(255, 0, 255));
	}

	void UIHUD::OnClear()
	{
	}
}