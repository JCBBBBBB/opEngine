#pragma once
#include "CommonInclude.h"

namespace op
{
	enum class KeyState
	{
		Down, // 이전 검사에서는 안눌렸고, 이번 검사에서는 눌림
		Pressed, // 이전 검사에 이어 계속 눌려있음
		Up, // 이전 검사에서는 눌렸고, 이번 검사에서 떼어짐
		None, // 계속 안 눌려있음
	};
	
	enum class KeyCode
	{
		Q, W, E, R, T, Y, U, I, O, P,
		A, S, D, F, G, H, J, K, L,
		Z, X, C, V, B, N, M,
		End,
	};


	class Input
	{
	public:
		struct Key
		{
			KeyCode keyCode; // 어떤 키인지
			KeyState keyState; // 이번 검사에서 계산한 상태
			bool isPressed; // 이전 검사에서 눌려 있었는지 판단하는데 사용할 값
		};

		static void Initialize();
		static void Update();

		static bool GetKeyDown(KeyCode code) { return m_keys[(UINT)code].keyState == KeyState::Down; }
		static bool GetKeyUp(KeyCode code) { return m_keys[(UINT)code].keyState == KeyState::Up; }
		static bool GetKey(KeyCode code) { return m_keys[(UINT)code].keyState == KeyState::Pressed; }


	private:
		static std::vector<Key> m_keys;
	};
}
