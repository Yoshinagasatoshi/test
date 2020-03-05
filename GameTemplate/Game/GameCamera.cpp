#include "stdafx.h"
#include "GameCamera.h"
GameCamera::GameCamera()
{
	
}
GameCamera::~GameCamera()
{

}
void GameCamera::Update()
{
	const float RotAmount = 5.0f;
	//�����_�̌v�Z
	m_target = m_player->GetPosition();
	CVector3 m_toCameraPosOld = m_toPos;
	//�p�b�h�̓���
	float x = g_pad->GetRStickXF();
	float y = g_pad->GetRStickYF();
	//Y���ł̉�]
	CQuaternion qRot;
	//�x�N�g���N���X��낤
	qRot.SetRotationDeg(m_AxisY, RotAmount * x);
	qRot.Multiply(m_toPos);
	//X���ł̉�]
	/*CVector3 axisX;
	axisX.Cross(CVector3::AxisY, m_toPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 2.0f * y);
	qRot.Multiply(m_toPos);*/
	//�J�����̃x�N�g����ۑ�

	//�J������]�̏�������߂�B
	CVector3 toPosDir = m_toPos;
	toPosDir.Normalize();
	if (toPosDir.y < 0.1f) {
		m_toPos = m_toCameraPosOld;
	}
	else if (toPosDir.y > 0.9f) {
		m_toPos = m_toCameraPosOld;
	}
	//���_�̌v�Z
	m_position = m_target + m_toPos;
	//Pos.y += 50.0f;
	//�^�[�Q�b�g��������������
	m_target.y += 100.0f;
	//���C���J�����̒����_�Ǝ��_��ݒ肷��B
	g_camera3D.SetTarget(m_target);
	g_camera3D.SetPosition(m_position);
	g_camera3D.Update();
}
void GameCamera::Draw()
{

}