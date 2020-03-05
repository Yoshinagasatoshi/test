#pragma once
#include "physics/PhysicsGhostObject.h"
#include "gameObject/ysGameObject.h"
class Player;
class Wepon_ghost : public IGameObject
{
public:
	Wepon_ghost();
	~Wepon_ghost();
	void Update();
	//bool Start();
	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	//eventName�͂����Ɠǂݍ��܂�Ă��邪�A
	//������^�𔻒f�ɂ���̂�����̂�
	//int�^�Ƃ��ɂ�������������������Ȃ�
	//�Ƃ肠�����ۗ��̃R�����g�A�E�g�B
	//void eventName(char* eventname)
	//{
	//	eventName = eventname;
	//}
	PhysicsGhostObject* GetGhostObject() {
		return &m_ghostObject;
	}
	void GhostInit();

private:
	PhysicsGhostObject m_ghostObject;
	CVector3 m_position;
	CQuaternion m_rotation = CQuaternion::Identity();
	//ghost�̃T�C�Y��ݒ�B* 100.0f
	CVector3 m_scale = CVector3::One();
	//char eventName;
	bool m_hanteifin = false;
	Player* m_player;
};

