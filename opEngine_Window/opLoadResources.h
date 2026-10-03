#pragma once

#include "..\\opEngine_SOURCE\\opResources.h"
#include "..\\opEngine_SOURCE\\opTexture.h"

namespace op
{
	void LoadResources()
	{
		Resources::Load<graphics::Texture>(L"BackGround", L"C:\\Workspace\\opEngine\\Resources\\CloudOcean.png");
	}
}