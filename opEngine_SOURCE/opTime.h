#pragma once
#include "CommonInclude.h"

namespace op
{
	class Time
	{
	public:
		static void Initialize();
		static void Update();
		static void Render(HDC hdc);

		// 이전 시간 갱신부터 이번 시간 갱신까지 지난 시간을 초 단위로 반환
		static float DeltaTime() { return DeltaTimeValue; }
	private:
		static LARGE_INTEGER CpuFrequency;
		static LARGE_INTEGER PrevFrequency;
		static LARGE_INTEGER CurrentFrequency;
		static float DeltaTimeValue;
	};
}
