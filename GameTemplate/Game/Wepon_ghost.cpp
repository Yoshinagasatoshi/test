#include "stdafx.h"
#include "Wepon_ghost.h"
#include "gameObject/ysGameObjectManager.h"

const float m_ghostscale = 150.0f;

Wepon_ghost::Wepon_ghost()
{
//	GhostInit();
}

Wepon_ghost::~Wepon_ghost()
{
	m_ghostObject.Release();
}

//bool Wepon_ghost::Start()
//{
//	return true;
//}

void Wepon_ghost::Update()
{
	GhostInit();
}

void Wepon_ghost::GhostInit()
{
	if (m_hanteifin) {
		DeleteGO(this);
	}
	if (!m_hanteifin) {
		m_ghostObject.CreateBox(
			m_position,
			m_rotation,
			m_scale * m_ghostscale
		);
		m_hanteifin = true;
	}
	
}