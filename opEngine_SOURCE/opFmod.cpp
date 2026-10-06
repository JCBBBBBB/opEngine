#include "opFmod.h"
#include "..\\External\\Include\\Fmod\\fmod_errors.h"

namespace op
{
	namespace
	{
		// FMOD 실패 원인을 메시지 창으로 표시
		bool CheckFmod(FMOD_RESULT result)
		{
			if (result == FMOD_OK)
			{
				return true;
			}

			MessageBoxA(
				nullptr,
				FMOD_ErrorString(result),
				"FMOD Error",
				MB_OK | MB_ICONERROR);

			return false;
		}
	}

	FMOD::Studio::System* Fmod::m_system = nullptr;
	FMOD::System* Fmod::m_coreSystem = nullptr;

	void Fmod::Initialize()
	{
		if (m_system)
		{
			return;
		}

		if (!CheckFmod(
			FMOD::Studio::System::create(&m_system)))
		{
			m_system = nullptr;
			return;
		}

		if (!CheckFmod(
			m_system->getCoreSystem(&m_coreSystem)))
		{
			Release();
			return;
		}

		if (!CheckFmod(
			m_coreSystem->setSoftwareFormat(
				0,
				FMOD_SPEAKERMODE_5POINT1,
				0)))
		{
			Release();
			return;
		}

		if (!CheckFmod(
			m_system->initialize(
				1024,
				FMOD_STUDIO_INIT_NORMAL,
				FMOD_INIT_NORMAL,
				nullptr)))
		{
			Release();
			return;
		}
	}

	void Fmod::Update()
	{
		if (m_system)
		{
			m_system->update();
		}
	}

	bool Fmod::CreateSound(
		const std::string& path,
		FMOD::Sound** sound)
	{
		if (!sound)
		{
			return false;
		}

		*sound = nullptr;

		if (!m_coreSystem)
		{
			return false;
		}

		return CheckFmod(
			m_coreSystem->createSound(
				path.c_str(),
				FMOD_3D,
				nullptr,
				sound));
	}

	void Fmod::SoundPlay(
		FMOD::Sound* sound,
		FMOD::Channel** channel)
	{
		if (!channel)
		{
			return;
		}

		*channel = nullptr;

		if (!m_coreSystem || !sound)
		{
			return;
		}

		if (!CheckFmod(
			m_coreSystem->playSound(
				sound,
				nullptr,
				false,
				channel)))
		{
			*channel = nullptr;
		}
	}

	void Fmod::Set3DListenerAttributes(
		const math::Vector2* pos)
	{
		if (!m_coreSystem || !pos)
		{
			return;
		}

		// 이번 원본은 전달받은 게임 좌표 대신 고정 좌표를 사용
		FMOD_VECTOR position = { 0.0f, 0.0f, 0.3f };
		FMOD_VECTOR velocity = { 0.0f, 0.0f, 0.0f };
		FMOD_VECTOR forward = { 0.0f, 0.0f, 1.0f };
		FMOD_VECTOR up = { 0.0f, 1.0f, 0.0f };

		m_coreSystem->set3DListenerAttributes(
			0,
			&position,
			&velocity,
			&forward,
			&up);
	}

	void Fmod::Release()
	{
		if (m_system)
		{
			m_system->release();
			m_system = nullptr;
		}

		m_coreSystem = nullptr;
	}
}