#pragma once

#include "..\\opEngine_SOURCE\\opResources.h"
#include "..\\opEngine_SOURCE\\opTexture.h"

namespace op
{
	void LoadResources()
	{
		Resources::Load<graphics::Texture>(L"Cat", L"..\\Resources\\chickenAlpha.bmp");
		Resources::Load<graphics::Texture>(
			L"Player",
			L"C:\\Workspace\\opEngine\\Resources\\Player.bmp");
		Resources::Load<graphics::Texture>(
			L"SpringFloor", L"..\\Resources\\SpringFloor.bmp");
		Resources::Load<graphics::Texture>(
			L"HPBAR",
			L"..\\Resources\\HPBAR.bmp");
		Resources::Load<graphics::Texture>(
			L"PixelMap",
			L"..\\Resources\\pixelMap.bmp"
		);
	}
}