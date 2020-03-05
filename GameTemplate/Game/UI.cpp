#include "stdafx.h"
#include "UI.h"
#include "gameObject/ysGameObjectManager.h"
/// <summary>
/// 今の所UIのコードを動かすとプログラムが死ぬので
/// 書いてはいるけど呼び出してはいません
/// </summary>
UI::UI()
{
	m_sprite.Init(L"Assets/ModelData/Green.png",1280.0f,720.0f);
}

UI::~UI()
{

}

void UI::Update()
{

}