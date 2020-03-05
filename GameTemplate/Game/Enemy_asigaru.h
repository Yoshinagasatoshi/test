#pragma once
#include "Enemy.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
#include "character/CharacterController.h"
#include "math/Matrix.h"
#include "physics/PhysicsGhostObject.h"
#include "graphics/Skeleton.h"
/// <summary>
/// Enemy�̔h���N���X���y
/// </summary>
class GameData;
class Wepon_ghost;
class Enemy_asigaru : public Enemy
{
public:
	Enemy_asigaru();
	~Enemy_asigaru();
	//�A�v�f
	void Update();
	//�`��
	void Draw();
	//����
	void Move();
	void Turn();
	//���񂾂Ƃ��̃��[��
	void DeadMove();
	//idlePos�̏�����
	void idlePosInit();
	//�����ɂ�锻�菈���֐�
	void StateJudge();
	//�S�[�X�g��Init
	//void ghostInit();

	//�ӂ��Ƃ΂��ʂ��󂯎��B
	void SetDamage(float recive)
	{
		m_recivePower = recive;
	}

	static Enemy_asigaru& GetInstans()
	{
		static Enemy_asigaru enemy;
		return enemy;
	}
	
	//PhysicsGhostObject* GetGhostObject(){
	//	return &m_ghostObject;
	//}
	//�L�����̏�Ԃ̎��
	enum AsigaruState {
		Asigaru_totugeki = 0,
		Asigaru_tikazuki,
		Asigaru_sentou,
		Asigaru_attack,
		Asigaru_damage,
		Asigaru_dead,
		Asigaru_anim_num
	};
	//���y�A�j��
	Animation m_asigaruAnime;
	//�F��ȑ��y�A�j�����i�[���Ă���z��
	AnimationClip m_asigaruAnimeClip[Asigaru_anim_num];
	struct IdlePos
	{
		//����̊i�[�ꏊ
		CVector3 idlePos;
		//�i�[�ꏊ���g���Ă��邩
		bool m_isUse = false;
		//���������s��ꂽ���H
		bool m_Syokika = false;
	};
	//�z�񐔂͑ҋ@�ꏊ�̐��B���܂�5
	static IdlePos m_idlePos[5];
	CVector3 m_moveSpeed = CVector3::Zero();
	CVector3 moveV = CVector3::Zero();
	int i = 0;
private:
	//�U�����Ԃ����b�����I����֐�
	//�v�����Ԃ�����������
	float AttackframeNum()
	{
		m_kougekiframenum = 100.0f + rand() % 500;
		return m_kougekiframenum;
	}
	//�A�j���[�V�����C�x���g���Ăяo����
	void OnAnimationEvent(const wchar_t* clipName, const wchar_t* eventName);
	//�L�����N�^�[�̃R���W����������
	void CharaconInit();
	//�����蔻����o���ϐ�
	//PhysicsGhostObject m_ghostObject;
	//���y�̏�ԊǗ��B
	AsigaruState m_asigaruState = Asigaru_totugeki;
	//���y�̑O�ɂ���Ă����X�e�[�g,�A�j���𗬂����̔���Ɏg�p
	AsigaruState m_oldState = Asigaru_sentou;

	//�L�����N�^�[�R���g���[���B�R���W�������ȒP�ɂ������
	CharacterController m_characon;

	//�L�����R��������B
	bool m_characonConUse = false;
	//�����ɏ����Ă���R�[�h
	bool m_isDeadfrag = false;
	//�U�����̃t���O
	bool m_isAttack = false;
	//�X�g�b�p�[�B��񂵂��Ă΂ꂽ���Ȃ��̂�
	bool m_deadMoveStopper = false;
	//�v���C���[������ł����Ԃ��ǂ������󂯎��ϐ��B
	//�֐��Ŏ󂯎��̂ŏ��������Ă܂���
	bool m_player_isdead;

	//�����̉�]
	//float m_modelRotate = 0.0f;
	//�v������
	float m_frameTimer = 0.0f;
	//�U�����ԁB���̎��Ԃ𒴂���ƍU���t���O������
	float m_kougekiframenum = 99.9f;
	//������܂ł̎���
	float m_Deathtimer_f = 0.0f;
	//������΂��ʂ����锠
	float m_recivePower;
	//�~�����B�K�v�Ȃ��߂����ɏ�����
	double PI = 3.14159265358979323846f;

	//�O����
	CVector3 m_forward = CVector3::AxisZ();

	//�������o������̃S�[�X�g�̃|�C���^�B
	Wepon_ghost* m_en_Wepon;
	//�X�P���g���B���̈ʒu��ghost���������߁B
	Skeleton* m_skeleton;
	//�Q�[���f�[�^�̃|�C���^�B���l���������甲�����Ă����B
	GameData* gamedata = nullptr;
};

