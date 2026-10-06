#pragma once
#include "CommonInclude.h"

#include "..\\External\\Include\\Fmod\\fmod_studio.hpp"
#include "..\\External\\Include\\Fmod\\fmod.hpp"

#ifdef _DEBUG
#pragma comment(lib, "fmodL_vc.lib")
#pragma comment(lib, "fmodstudioL_vc.lib")
#else
#pragma comment(lib, "fmod_vc.lib")
#pragma comment(lib, "fmodstudio_vc.lib")
#endif

namespace op
{
	class Fmod
	{
	public:
		static void Initialize();
		static void Update();

		static bool CreateSound(
			const std::string& path,
			FMOD::Sound** sound);

		static void SoundPlay(
			FMOD::Sound* sound,
			FMOD::Channel** channel);

		static void Set3DListenerAttributes(
			const math::Vector2* pos);

		static void Release();

	private:
		static FMOD::Studio::System* m_system;
		static FMOD::System* m_coreSystem;
	};
}