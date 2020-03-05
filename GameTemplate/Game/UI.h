#pragma once
#include "graphics/Sprite.h"
#include "gameObject/ysGameObject.h"
//UIクラス。体力とか無双ゲージとかつくるぞ
class UI : public IGameObject
{
public:
	UI();
	~UI();
	void Update();
private:
	Sprite m_sprite;
};