#pragma once
#include "graphics/Sprite.h"
#include "gameObject/ysGameObject.h"
//UI�N���X�B�̗͂Ƃ����o�Q�[�W�Ƃ����邼
class UI : public IGameObject
{
public:
	UI();
	~UI();
	void Update();
private:
	Sprite m_sprite;
};