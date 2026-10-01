#include "opInput.h"

namespace op
{
	std::vector<Input::Key> Input::m_keys = {};

	int ASCII[(UINT)KeyCode::End] =
	{
		'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
		'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
		'Z', 'X', 'C', 'V', 'B', 'N', 'M',
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
}