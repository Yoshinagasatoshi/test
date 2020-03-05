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
	SkinModel m_model;	//�X�L�����f���B
	CVector3 m_position = CVector3{ 0.0f,-100.0f,0.0f }; //�w�i�̈ʒu
	CQuaternion m_rotation = CQuaternion::Identity();
	CVector3 m_scale = CVector3::One(); //�w�i�̑傫��
	PhysicsStaticObject m_physicsStaticObject;
};

