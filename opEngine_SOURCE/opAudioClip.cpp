#include "opAudioClip.h"

namespace op
{
	AudioClip::AudioClip()
		: Resource(enums::ResourceType::AudioClip)
		, m_sound(nullptr)
		, m_channel(nullptr)
		, m_minDistance(1.0f)
		, m_maxDistance(1000.0f)
		, m_loop(false)
	{
	}

	AudioClip::~AudioClip()
	{
		if (m_sound)
		{
			m_sound->release();
			m_sound = nullptr;
		}

		m_channel = nullptr;
	}

	HRESULT AudioClip::Load(const std::wstring& path)
	{
		// 이번 원본의 영문 파일 경로를 문자열로 변환
		std::string filePath(path.begin(), path.end());

		if (!Fmod::CreateSound(filePath, &m_sound))
		{
			return E_FAIL;
		}

		if (m_sound->set3DMinMaxDistance(
			m_minDistance,
			m_maxDistance) != FMOD_OK)
		{
			return E_FAIL;
		}

		return S_OK;
	}

	void AudioClip::Play()
	{
		if (!m_sound)
		{
			return;
		}

		if (m_loop)
		{
			m_sound->setMode(FMOD_LOOP_NORMAL);
		}
		else
		{
			m_sound->setMode(FMOD_LOOP_OFF);
		}

		Fmod::SoundPlay(m_sound, &m_channel);
	}

	void AudioClip::Stop()
	{
		if (m_channel)
		{
			m_channel->stop();
			m_channel = nullptr;
		}
	}

	void AudioClip::Set3DAttributes(
		const math::Vector2 pos)
	{
		if (!m_channel)
		{
			return;
		}

		// 이번 원본은 소리 위치도 고정 좌표를 사용
		FMOD_VECTOR position = { 0.0f, 0.0f, 0.0f };
		FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };

		m_channel->set3DAttributes(
			&position,
			&velocity);
	}
}