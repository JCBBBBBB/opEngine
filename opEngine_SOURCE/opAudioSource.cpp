#include "opAudioSource.h"
#include "opTransform.h"
#include "opGameObject.h"

namespace op
{
	AudioSource::AudioSource()
		: Component(enums::ComponentType::AudioSource)
		, m_audioClip(nullptr)
	{
	}

	AudioSource::~AudioSource()
	{
	}

	void AudioSource::Initialize()
	{
	}

	void AudioSource::Update()
	{
	}

	void AudioSource::LateUpdate()
	{
		if (!m_audioClip || !GetOwner())
		{
			return;
		}

		Transform* transform =
			GetOwner()->GetComponent<Transform>();

		if (!transform)
		{
			return;
		}

		math::Vector2 pos = transform->GetPosition();

		m_audioClip->Set3DAttributes(pos);
	}

	void AudioSource::Render(HDC hdc)
	{
	}

	void AudioSource::Play()
	{
		if (m_audioClip)
		{
			m_audioClip->Play();
		}
	}

	void AudioSource::Stop()
	{
		if (m_audioClip)
		{
			m_audioClip->Stop();
		}
	}

	void AudioSource::SetLoop(bool loop)
	{
		if (m_audioClip)
		{
			m_audioClip->SetLoop(loop);
		}
	}
}