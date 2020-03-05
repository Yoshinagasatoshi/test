#pragma once
#include "Enemy.h"
#include "character/CharacterController.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
class Enemy_Busyo :
	public Enemy
{
public:
	Enemy_Busyo();
	~Enemy_Busyo();

	void SetPosition(CVector3 pos)
	{
		m_position = pos;
	}
	CVector3 Getposition()
	{
		return m_position;
	}
	void Draw();
	void NormalMove();
	void IdleMove();
	void AttackMove();
	void Update();
	void CharaconInit();
	void StateJudge();
	void ThisDamage();
	void ThisDelete();
private:
	//�U�����Ԃ����b�����I����֐�
	//�v�����Ԃ�����������
	float AttackframeNum()
	{
		m_attackFrameNum = 500.0f + rand() % 200;
		return m_attackFrameNum;
	}
	enum State
	{
		IDL = 0,
		ATK,
		MOVE,
		DAMAGE,
		DEATH,
		AnimationClip_Num
	};
	Animation m_enemy_BusyoAnime;
	AnimationClip m_animClip[AnimationClip_Num];
	CVector3 distance;			//���̂܂܁I�����ł��B
	State m_state;				//���̂܂�
	SkinModel m_model;			//�X�L�����f���B
	CharacterController m_characon;//�L�����R��
	CVector3 m_moveSpeed = CVector3::Zero();//�ړ��ʂ�����ϐ��B
	bool m_charaConUse = false;//�L�����R���������Ă��邩�m�F����ϐ��B
	bool m_isDeadfrag = false;

	int m_HP = 25;					//��
	float m_attackFrameNum = 0.0f;	//�U�����ԁB���̎��Ԃ𒴂���ƍU���t���O������
	float m_frameTimer = 0.0f;		//���Ԃ�}�邽��
	//�������o������̃S�[�X�g�̃|�C���^�B
	Wepon_ghost* m_en_Wepon;
};

