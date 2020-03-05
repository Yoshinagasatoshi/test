#include "stdafx.h"
#include "Player.h"
#include <iostream>
#include "gameObject/ysGameObjectManager.h"
#include "Enemy.h"
#include "Enemy_asigaru.h"
#include "Wepon_ghost.h"

const float posClearRange = 600.0f * 600.0f;	//�N���A������s���͈́B
const float PLAYER_COLLIDER_HEIGHT = 100.0f;	//�v���C���[�̃J�v�Z���R���C�_�[�̍����B
const float PLAYER_COLLIDER_RADIUS = 60.0f;		//�v���C���[�̃J�v�Z���R���C�_�[�̔��a�B

const float SpeedAmount = 1500.0f;				//���ʂ̈ړ���
const float gravity = 200.0f;					//�d��
const float JumpPower = 1200.0f;				//�v���C���[�̔�ԗ�
const float standardPower = 200.0f;				//�v���C���[�̓G������΂���
const float limit = 2.0f;						//�d�͌W���̏��
const int Timer_ZERO = 0;						//0�ɂȂ�B���̂܂�

Player::Player()
{
	CharaconInit();
	//cmo�t�@�C���̓ǂݍ��݁B
	m_playerModel.Init(L"Assets/modelData/busyo.cmo");
	//�ړ���Ԃ̃��[�h
	m_busyoAnimeClip[animClip_idle].Load(L"Assets/animData/busyo_idle.tka");
	m_busyoAnimeClip[animClip_idle].SetLoopFlag(true);
	m_busyoAnimeClip[animClip_Walk].Load(L"Assets/animData/busyo_walk.tka");
	m_busyoAnimeClip[animClip_Walk].SetLoopFlag(true);
	//�U���A�j�����[�h
	m_busyoAnimeClip[animClip_ATK1].Load(L"Assets/animData/busyo_kougeki.tka");
	m_busyoAnimeClip[animClip_ATK2].Load(L"Assets/animData/busyo_kougeki2.tka");
	m_busyoAnimeClip[animClip_ATK3].Load(L"Assets/animData/busyo_kougeki3.tka");
	m_busyoAnimeClip[animClip_ATK4].Load(L"Assets/animData/busyo_kougeki4.tka");
	m_busyoAnimeClip[animClip_ATK5].Load(L"Assets/animData/busyo_kougeki5.tka");
	//�_���[�W���[�h
	m_busyoAnimeClip[animClip_SmallDamage].Load(L"Assets/animData/busyo_smalldamage.tka");
	m_busyoAnimeClip[animClip_busyo_dead].Load(L"Assets/animData/busyo_dead.tka");
	//�S�����̍Đ��ł����̂�false�ɂ��Ƃ�
	for (int i = animClip_ATK1; i < animClip_num; i++) {
		m_busyoAnimeClip[i].SetLoopFlag(false);
	}
	//�A�j���[�V�����惂�f���ƃA�j���[�V�����̌���ݒ�B
	m_busyoAnime.Init(
		m_playerModel,
		m_busyoAnimeClip,
		animClip_num
	);

	m_skelton = &m_playerModel.GetSkeleton();
	//���̃R�[�h�̓{�[���̔z����m�F���邽�߂ɏ����Ă���R�[�h�B���ڃQ�[���ɂ͊ւ���ĂȂ��B
	//�Ƃ肠����30������z����쐬
	const wchar_t* bonename[30];

	for (int i = 0; i < 29; i++) {
		bonename[i] = m_skelton->GetBone(i)->GetName();

		if (i == 28)
		{
			bonename[i + 1] = L"end";
		}
	}

	m_busyoAnime.AddAnimationEventListener(	[&](const wchar_t* clipName, const wchar_t* eventName)
	{
		auto m_bone = m_skelton->GetBone(20);
		CVector3 bonepos;
		//y�������������{�[�����Z�b�g
		bonepos.Set(
			m_bone->GetWorldMatrix().m[3][0],
			0,
			m_bone->GetWorldMatrix().m[3][2]
		);
		//OnAnimationEvent(clipName,eventName);

		//�{�[����pos�ƃv���C���[��pos�𑫂����ꏊ
		m_calcPos = m_position;
		//ghost���������܂��Ă����̂ŏ�����ɍ��킹��B
		m_calcPos.y += 70.0f;

		m_pl_Wepon = g_goMgr.NewGameObject<Wepon_ghost>("PL_Wepon");
		m_pl_Wepon->SetPosition(m_calcPos);
		m_pl_Wepon->GhostInit();
	}
	);
}


Player::~Player()
{
}

void Player::CharaconInit()
{
	//�L�����R���̏�����
	m_characon.Init(
		PLAYER_COLLIDER_RADIUS,//�R���C�_�[�̔��a�̒���
		PLAYER_COLLIDER_HEIGHT,//�R���C�_�[�̍���
		m_position
	);
}

void Player::Update()
{
	if (m_busyoState != BusyoDead) {
		//�n�ʂ��Ă�H
		if (m_characon.IsOnGround()) {
			//�U������Ƃ��̃��[�V����
			AttackMove();
			//�d�͂̏d�݂��y������
			m_gravity_keisuu = 0.1f;
			//�W�����v���Ă��H
			if (m_Jumpfrag) {
				m_Jumpfrag = false;
			}
		}
		//�X�e�[�g���Ƃɂ̏����Ɍ�ł���B
		if (m_busyoState != BusyoAttack) {
			if (g_pad->IsTrigger(enButtonA)) {
				if (!m_Jumpfrag) {
					m_moveSpeed.y += JumpPower;
					m_Jumpfrag = true;
					m_animStep = 0;
				}
			}
			m_gravity_keisuu += 0.1f;
			if (m_gravity_keisuu > limit) {
				m_gravity_keisuu = limit;
			}
			m_busyoAnime.Play(animClip_idle, 0.5f);
		}
		//������ӂ̏����ł͂ق��Ɋ֐����g����������������
		if (m_damagefrag)
		{
			m_damagefrag = false;
			if (m_PL_HP != 0) {
				m_PL_HP--;
			}
			else {
				//m_deadFrag = true;
				m_busyoState = BusyoDead;
			}
			m_animState = animClip_SmallDamage;
			m_busyoAnime.Play(animClip_SmallDamage, 0.5f);
		}

		//�_���[�W�A�j���[�V�������I������痧���p��
		if (!m_busyoAnime.IsPlaying())
		{
			m_animState = animClip_idle;
			m_busyoState = BusyoNormal;
			//if (m_underAttack) {
			//	m_underAttack = false;
			//}
			m_busyoAnime.Play(animClip_idle, 0.2f);
		}
		//�ړ�����
		Move();
		//��]����
		Turn();
		//����̃S�[�X�g�����������ɓ���������A���񂾂Ƃ����M���𗧂Ă�
		QueryGOs<Wepon_ghost>("EN_Wepon", [&](Wepon_ghost* wepon) {
			PhysicsGhostObject* ghostobject = wepon->GetGhostObject();
			g_physics.ContactTest(m_characon, [&](const btCollisionObject& contactObject) {
				if (ghostobject->IsSelf(contactObject) == true) {
					//�ʂ��Ă���̂͊m�F����
					PlayerDamage();

					g_Effect.m_playEffectHandle = g_Effect.m_effekseerManager->Play(g_Effect.m_sampleEffect, m_position.x, m_position.y + 100.0f, m_position.z);
				}
				});
			return true;
			});
	}
	else {
		//�v���C���[������ł��鎞�̏���
		m_moveSpeed = CVector3::Zero();
		m_busyoAnime.Play(animClip_busyo_dead);
	}
	Execute();
}

void Player::Move()
{
	//�v���C���[������ł��Ȃ����̏����B
	//���ʂ̈ړ��ʂ̓A�v�f���ƂɃ��Z�b�g����B
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	if (m_busyoState != BusyoAttack) {
		//���͗ʂ��󂯎��
		WideMoveL = g_pad->GetLStickXF();
		heightMoveL = g_pad->GetLStickYF();

		//�J�����̑O�����ƉE�������擾
		m_CameraForward = g_camera3D.GetForword();
		m_CameraRight = g_camera3D.GetRight();
		//Y�̏��͂���Ȃ��̂�0�ɂ��A�O�ƉE�����̒P�ʂƂ���B
		m_CameraForward.y = 0.0f;
		m_CameraForward.Normalize();
		m_CameraRight.y = 0.0f;
		m_CameraRight.Normalize();
		//�U�����͎��R�ɓ����Ȃ����ɂ���B
		//m_busyoState = BusyoAttack;
		if (!m_underAttack) {
			m_moveSpeed += m_CameraForward * heightMoveL * SpeedAmount;
			m_moveSpeed += m_CameraRight * WideMoveL * SpeedAmount;
		}
	}
	m_moveSpeed.y -= gravity * m_gravity_keisuu;
}

void Player::Draw()
{
	//�v���C���[��`��
	m_playerModel.Draw(
		g_camera3D.GetViewMatrix(), 
		g_camera3D.GetProjectionMatrix()
	);
}


void Player::Turn()
{
	if (m_busyoState != BusyoAttack) {

		if (fabsf(m_moveSpeed.x) <= 0.001f    //fabsf�͐�Βl�Bm_movespeed.x&m_movespeedz��
			&& fabsf(m_moveSpeed.z) <= 0.001f) {//0.001�ȉ��̎��ɂ͉������Ȃ��B
			return;
		}
		else {
			float angle = atan2(m_moveSpeed.x, m_moveSpeed.z);
			m_rotation.SetRotation(CVector3::AxisY(), angle);
		}
	}
}

void Player::AttackMove()
{
	//��Ԏ���
	float InterpolationTime = 0.2f;
	if (g_pad->IsTrigger(enButtonX)&&m_playTimer>3.0f) {
	//m_busyoState = BusyoAttack;
		if (!m_underAttack)
		{
			m_underAttack = true;
		}
		m_busyoState = BusyoAttack;
		//���肵�܂��B
		//�X�g���e�W�[�p�^�[���\���R
		switch (m_animStep)
		{
		case animClip_idle:
			m_busyoAnime.Play(animClip_ATK1, InterpolationTime);
			m_busyoAnimeClip->GetKeyFramePtrListArray();
			//enmu�̗��ꂽ�ʒu�ɃA�^�b�N�����邽�߁A�ŏ�����+= animClip_ATK1�𑫂�
			m_animStep += animClip_ATK1;
			m_blowOffPower = standardPower * 1.7f;
			break;
		case animClip_ATK1:
			m_busyoAnime.Play(animClip_ATK2, InterpolationTime);
			m_animStep++;
			break;
		case animClip_ATK2:
			m_busyoAnime.Play(animClip_ATK3, InterpolationTime);
			m_animStep++;
			break;
		case animClip_ATK3:
			m_busyoAnime.Play(animClip_ATK4, InterpolationTime);
			m_animStep++;
			m_blowOffPower = standardPower * 2.0f;
			break;
		case animClip_ATK4:
			m_busyoAnime.Play(animClip_ATK5, InterpolationTime);
			m_animStep++;
			m_blowOffPower = standardPower * 2.5f;
			break;
		}
	}
	m_playTimer++;
	if (m_animStep != 0) {
		if (m_animStep != m_oldAnimStep) {
			m_playTimer = Timer_ZERO;
			m_oldAnimStep = m_animStep;
		}
		//�Ō�܂ōs���ƌ������炷���Ԃ𑝂₷
		if (m_animStep == animClip_ATK5) {
			//���₷����
			m_TimerRelease = 30;
		}
		if (m_playTimer >= m_TimerRelease) {
			//���̎��Ԃ��߂�����A�j���X�e�[�g�֌W��������
			m_busyoState = BusyoNormal;
			m_TimerRelease = 20;
			m_animStep = animClip_idle;
			m_oldAnimStep = animClip_idle;
			m_playTimer = Timer_ZERO;
			m_busyoAnime.Play(animClip_idle, InterpolationTime*2.0f);
			m_underAttack = false;
		}
	}
}

void Player::Execute()
{
	
	if (m_pl_Wepon != nullptr) {
		m_pl_Wepon->SetPosition(m_position);
	}
	
	//���[���h���W�̍X�V�@��������skeletonUpdate��������
	auto footStep = m_busyoAnime.Update(1.0f / 30.0f);//���[�J�����W�̍X�V�@�������͂�����Ȃ�
	//if (m_busyoState == BusyoAttack) {
	//�U�����̓t�b�g�X�e�b�v�̈ړ��ʂ����Z����B
	CMatrix mBias = CMatrix::Identity();
	mBias.MakeRotationX(CMath::PI * -0.5f);
	CMatrix rotMatrix;
	//��]�s����쐬����B
	rotMatrix.MakeRotationFromQuaternion(m_rotation);
	rotMatrix.Mul(mBias, rotMatrix);
	rotMatrix.Mul(footStep);
	footStep *= 60.0f;
	m_moveSpeed += footStep;
	//}
	m_position = m_characon.Execute(1.0f / 60.0f, m_moveSpeed);
	//���[���h�s��̍X�V�B
	m_playerModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}


int Player::RequestEnemyData(CVector3 pos,Enemy* enemy)
{
	for (int i = 0; i < DestinationNum; i++)
	{
		//��ԍŏ��ɃG�l�~�[�̋󂢂Ă��鏊�ɏ�������
		if (m_enemydata[i].position.y == 0.0f) {
			m_enemydata[i].position = pos;
			m_enemydata[i].enemy = enemy;
			return i;
		}
		else if (m_enemydata[i].enemy == enemy) {
			return i;
		}

		if (m_enemydata[i].position.y != 0.0f) {
			CVector3 kyori = m_enemydata[i].position - pos;
			if (kyori.LengthSq() > posClearRange) {
				m_enemydata[i].enemy = NULL;
				m_enemydata[i].position = CVector3{0.0f,0.0f,0.0f};
				return -1;
			}
		}
	}
	//null�łȂ�����
	//����������Ă�����
	//-1��߂�l�ɂ���
	return -1;
}