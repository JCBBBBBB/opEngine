#include "opInput.h"
#include "opApplication.h"

extern op::Application application;

namespace op
{
	std::vector<Input::Key> Input::m_keys = {};
	math::Vector2 Input::m_mousePosition = math::Vector2::One;

	int ASCII[(UINT)KeyCode::End] =
	{
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
		'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
		'Z', 'X', 'C', 'V', 'B', 'N', 'M',
		VK_LEFT, VK_RIGHT, VK_DOWN, VK_UP,
		VK_LBUTTON, VK_MBUTTON, VK_RBUTTON,
		VK_SPACE,
	};

	// 26개 키 각각의 정보를 만들어 m_keys에 추가
	void Input::Initialize()
	{
		for (size_t i = 0; i < (UINT)KeyCode::End; i++)
		{
			Key key = {};
			key.isPressed = false;
			key.keyState = KeyState::None;
			key.keyCode = (KeyCode)i;

			m_keys.push_back(key);
		}
	}

	// 이전 상태와 현재 입력을 비교
	/*전에 눌렸는가 ? 지금 눌렸는가 ? 새 상태
		아니오			예			Down
		예				예			Pressed
		예				아니오		Up
		아니오			아니오		None*/
	void Input::Update()
	{
		GetMousePositionByWindow();

		for (size_t i = 0; i < m_keys.size(); i++)
		{
			// 키가 눌렸다
			if (GetAsyncKeyState(ASCII[i]) & 0x8000)
			{
				if (m_keys[i].isPressed == true)
				{
					m_keys[i].keyState = KeyState::Pressed;
				}
				else
				{
					m_keys[i].keyState = KeyState::Down;
				}

				m_keys[i].isPressed = true;
			}
			else
			{
				if (m_keys[i].isPressed == true)
				{
					m_keys[i].keyState = KeyState::Up;
				}
				else
				{
					m_keys[i].keyState = KeyState::None;
				}

				m_keys[i].isPressed = false;
			}
		}
	}

	void Input::GetMousePositionByWindow()
	{
		// 메인 창 밖이면 사용할 수 없는 좌표로 표시
		m_mousePosition = math::Vector2(-1.0f, -1.0f);

		POINT mousePos = {};
		HWND hwnd = application.GetHwnd();

		if (!hwnd
			|| !GetCursorPos(&mousePos)
			|| !ScreenToClient(hwnd, &mousePos))
		{
			return;
		}

		const LONG width = static_cast<LONG>(application.GetWidth());
		const LONG height = static_cast<LONG>(application.GetHeight());

		if (mousePos.x >= 0 && mousePos.x < width
			&& mousePos.y >= 0 && mousePos.y < height)
		{
			m_mousePosition.x = static_cast<float>(mousePos.x);
			m_mousePosition.y = static_cast<float>(mousePos.y);
		}
	}

}