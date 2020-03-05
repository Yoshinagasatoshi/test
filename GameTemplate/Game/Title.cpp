#include "stdafx.h"
#include "Title.h"
#include "Game.h"
#include "gameObject/ysGameObjectManager.h"
Title::Title()
{
	m_sprite.Init(L"Assets/sprite/Title.dds",1280.0f,720.0f);
}

Title::~Title()
{
	
}

void Title::Update()
{
	if (g_pad->IsTrigger(enButtonA)) {
		g_goMgr.DeleteGOObject(this);
		Game* game = g_goMgr.NewGameObject<Game>("Game");
	}
}