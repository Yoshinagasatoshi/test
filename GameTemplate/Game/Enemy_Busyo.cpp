#include "stdafx.h"
#include "gameObject/ysGameObjectManager.h"
#include "Wepon_ghost.h"
#include "Enemy_Busyo.h"

const float power = 500.0f;

Enemy_Busyo::Enemy_Busyo()
{
	//���f�����Ԃ�����H
	//�Ƃ肠�������̔��A��ō����ւ���B�B
	m_model.Init(L"Assets/modelData/enemy_busyo.cmo");
	m_rotation = CQuaternion::Identity();
	m_scale = CVector3::One();
	m_animClip[ATK].Load(L"Assets/animData/enemy_Busyo_Attack.tka");
	m_animClip[IDL].Load(L"Assets/animData/enemy_Busyo_Idle.tka");
	m_animClip[MOVE].Load(L"Assets/animData/enemy_Busyo_Inflate.tka");
	m_animClip[DAMAGE].Load(L"Assets/animData/enemy_Busyo_Damage.tka");
	m_animClip[DEATH].Load(L"Assets/animData/enemy_Busyo_Death.tka");

	m_enemy_BusyoAnime.Init(
		m_model,
		m_animClip,
		AnimationClip_Num
	);

	m_animClip[ATK].SetLoopFlag(false);
	m_animClip[MOVE].SetLoopFlag(false);
	m_animClip[IDL].SetLoopFlag(true);
	m_animClip[DAMAGE].SetLoopFlag(false);
	m_animClip[DEATH].SetLoopFlag(false);

	m_enemy_BusyoAnime.AddAnimationEventListener([&](const wchar_t* clipName, const wchar_t* eventName) {
		(void)clipName;
		m_en_Wepon = g_goMgr.NewGameObject<Wepon_ghost>("EN_Wepon");
		m_en_Wepon->SetPosition(m_position);
		}
	);
}

Enemy_Busyo::~Enemy_Busyo()
{

}

void Enemy_Busyo::Update()
{
	//�L�����R���������Ă��Ȃ�����������B
	if (!m_charaConUse) {
		CharaconInit();
		m_charaConUse = true;
	}
	//m_state�W���b�W�B���̒���m_state������U����B
	StateJudge();

	switch (m_state)
	{
	case ATK:
		AttackMove();
		break;
	case MOVE:
		NormalMove();
		break;
	case IDL:
		IdleMove();
		break;
	case DAMAGE:
		ThisDelete();
	}
	//���߁[�������
	ThisDamage();

	//���[���h���W�̍X�V�@��������skeletonUpdate��������
	auto footStep = m_enemy_BusyoAnime.Update(1.0f / 30.0f);//���[�J�����W�̍X�V�@�������͂�����Ȃ�
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

	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	m_enemy_BusyoAnime.Update(1/30.0f);
}

void Enemy_Busyo::CharaconInit()
{
	//�L�����R���̏�����
	m_characon.Init(
		60.0f, //���a
		100.0f,//����
		m_position//�ʒu
	);
}

void Enemy_Busyo::Draw()
{
	m_model.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}


void Enemy_Busyo::StateJudge()
{
	distance = m_player->GetPosition() - m_position;
	if (distance.LengthSq() < BattleRange) {
		m_state = ATK;
	}
	if (distance.LengthSq() > BattleRange
		&& distance.LengthSq() < VigilanceRange) {
		m_state = MOVE;
	}
	if (distance.LengthSq() > VigilanceRange) {
		m_state = IDL;
	}
	//�_���[�W���󂯂Ă��鎞�͑��̂̃X�e�[�g�����D��
	if (m_isDeadfrag) {
		m_state = DAMAGE;
	}
}



//�U���͈͂ɗ����炱���ɓ���B
void Enemy_Busyo::AttackMove()
{
	m_frameTimer += 1.0f;

	//�U�����̓���
	m_moveSpeed = CVector3::Zero();
	//���[���h���W�̍X�V
	m_moveSpeed.y += grabity;
	if (m_characon.IsOnGround()) {
		m_moveSpeed.y = 0.0f;
	}
	//�U���̊��o�����߂�
	AttackframeNum();
	if (m_frameTimer > m_attackFrameNum)
	{
		m_enemy_BusyoAnime.Play(ATK, 0.2f);
		if (!m_enemy_BusyoAnime.IsPlaying())
		{
			m_frameTimer = 0;
			m_enemy_BusyoAnime.Play(MOVE, 0.2f);
		}
	}
	m_position = m_characon.Execute(1.0f / 30.0f, m_moveSpeed);
}

//���G�͈͂ɗ����炱���ɓ���
void Enemy_Busyo::NormalMove()
{
	CVector3 direction = distance;
	direction.Normalize();
	m_moveSpeed = direction * power;
	//���[���h���W�̍X�V
	m_moveSpeed.y += grabity;
	if (m_characon.IsOnGround()) {
		m_moveSpeed.y = 0.0f;
	}

	m_enemy_BusyoAnime.Play(MOVE,0.2f);
	m_position = m_characon.Execute(1.0f / 30.0f, m_moveSpeed);
}

//���������������炱���ɓ���B
void Enemy_Busyo::IdleMove()
{
	m_moveSpeed = CVector3::Zero();
	//���[���h���W�̍X�V
	m_moveSpeed.y += grabity;
	if (m_characon.IsOnGround()) {
		m_moveSpeed.y = 0.0f;
	}

	m_enemy_BusyoAnime.Play(IDL, 0.2f);
	m_position = m_characon.Execute(1.0f / 30.0f, m_moveSpeed);
}

void Enemy_Busyo::ThisDamage()
{
	//����̃S�[�X�g�����������ɓ���������A���񂾂Ƃ����M���𗧂Ă�
	QueryGOs<Wepon_ghost>("PL_Wepon", [&](Wepon_ghost* wepon) {
		PhysicsGhostObject* ghostobject = wepon->GetGhostObject();
		g_physics.ContactTest(m_characon, [&](const btCollisionObject& contactObject) {
			if (ghostobject->IsSelf(contactObject) == true) {

				//�ʂ��Ă���̂͊m�F����
				m_isDeadfrag = true;

				//�G�t�F�N�g���o���B
				g_Effect.m_playEffectHandle = g_Effect.m_effekseerManager->Play(
					g_Effect.m_sampleEffect,
					m_position.x, m_position.y + 100.0f,
					m_position.z
				);

			}
			});
		return true;
		});
}

void Enemy_Busyo::ThisDelete()
{
	//�̗͂͂܂��c���Ă��邩�ǂ���
	if (m_HP != 0) {
		m_HP--;
		m_enemy_BusyoAnime.Play(DAMAGE, 0.2f);
		m_isDeadfrag = false;
	}
	//�Ȃ��Ȃ��Ă����炱�����ʂ�
	else {
		m_enemy_BusyoAnime.Play(DEATH, 0.1f);
		if (!m_enemy_BusyoAnime.IsPlaying()) {
			DeleteGO(this);
		}
	}
}
