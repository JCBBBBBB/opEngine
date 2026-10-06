#include "opAudioListener.h"
#include "opTransform.h"
#include "opGameObject.h"
#include "opFmod.h"

namespace op
{
	AudioListener::AudioListener()
		: Component(enums::ComponentType::AudioListener)
	{
	}

	AudioListener::~AudioListener()
	{
	}

	void AudioListener::Initialize()
	{
	}

	void AudioListener::Update()
	{
	}

	void AudioListener::LateUpdate()
	{
		if (!GetOwner())
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

		Fmod::Set3DListenerAttributes(&pos);
	}

	void AudioListener::Render(HDC hdc)
	{
	}
}