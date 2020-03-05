#pragma once
#include "gameObject/ysGameObject.h"
#include "physics/PhysicsStaticObject.h"
class BackGround : public IGameObject
{
public:
	BackGround();
	~BackGround();
	void Update();
	void Draw();
private:
	SkinModel m_model;	//スキンモデル。
	CVector3 m_position = CVector3{ 0.0f,-100.0f,0.0f }; //背景の位置
	CQuaternion m_rotation = CQuaternion::Identity();
	CVector3 m_scale = CVector3::One(); //背景の大きさ
	PhysicsStaticObject m_physicsStaticObject;
};

