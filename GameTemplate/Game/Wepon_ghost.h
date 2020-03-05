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
	//eventNameはちゃんと読み込まれているが、
	//文字列型を判断にするのが難しいので
	//int型とかにした方がいいかもしれない
	//とりあえず保留のコメントアウト。
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
	//ghostのサイズを設定。* 100.0f
	CVector3 m_scale = CVector3::One();
	//char eventName;
	bool m_hanteifin = false;
	Player* m_player;
};

