#pragma once
#include "opFmod.h"
#include "opResource.h"

namespace op
{
	class AudioClip : public Resource
	{
	public:
		AudioClip();
		~AudioClip() override;

		HRESULT Load(const std::wstring& path) override;

		void Play();
		void Stop();

		void Set3DAttributes(const math::Vector2 pos);
		void SetLoop(bool loop) { m_loop = loop; }

	private:
		FMOD::Sound* m_sound;     // 읽어 둔 소리 데이터
		FMOD::Channel* m_channel; // 재생을 제어할 채널

		float m_minDistance;
		float m_maxDistance;
		bool m_loop;
	};
}