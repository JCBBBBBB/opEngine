#pragma once

#include "..\\opEngine_SOURCE\\opResources.h"
#include "..\\opEngine_SOURCE\\opTexture.h"

namespace op
{
	void LoadResources()
	{
		//Resources::Load<graphics::Texture>(L"BackGround", L"C:\\Workspace\\opEngine\\Resources\\CloudOcean.png");
		Resources::Load<graphics::Texture>(L"Map", L"..\\Resources\\img\\map\\map_pure.png");
		Resources::Load<graphics::Texture>(L"PackMan", L"..\\Resources\\img\\pacman\\0.png");
		Resources::Load<graphics::Texture>(L"Chicken", L"..\\Resources\\chicken.bmp");
		Resources::Load<graphics::Texture>(L"Cat", L"..\\Resources\\chickenAlpha.bmp");
		Resources::Load<graphics::Texture>(L"Bubble", L"..\\Resources\\Bubble.png");
		Resources::Load<graphics::Texture>(L"MapleEffect", L"..\\Resources\\ezgif.com-gif-maker.png");
		
	}
}