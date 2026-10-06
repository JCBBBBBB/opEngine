#pragma once
#include "opComponent.h"
#include "opAudioClip.h"

namespace op
{
	class AudioSource : public Component
	{
	public:
		AudioSource();
		~AudioSource() override;

		void Initialize() override;
		void Update() override;
		void LateUpdate() override;
		void Render(HDC hdc) override;

		void Play();
		void Stop();
		void SetLoop(bool loop);

		void SetClip(AudioClip* clip) { m_audioClip = clip; }
		AudioClip* GetClip() { return m_audioClip; }

	private:
		AudioClip* m_audioClip;
	};
}