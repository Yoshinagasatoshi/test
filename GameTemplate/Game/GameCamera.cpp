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
	//注視点の計算
	m_target = m_player->GetPosition();
	CVector3 m_toCameraPosOld = m_toPos;
	//パッドの入力
	float x = g_pad->GetRStickXF();
	float y = g_pad->GetRStickYF();
	//Y軸での回転
	CQuaternion qRot;
	//ベクトルクラス作ろう
	qRot.SetRotationDeg(m_AxisY, RotAmount * x);
	qRot.Multiply(m_toPos);
	//X軸での回転
	/*CVector3 axisX;
	axisX.Cross(CVector3::AxisY, m_toPos);
	axisX.Normalize();
	qRot.SetRotationDeg(axisX, 2.0f * y);
	qRot.Multiply(m_toPos);*/
	//カメラのベクトルを保存

	//カメラ回転の上限を決める。
	CVector3 toPosDir = m_toPos;
	toPosDir.Normalize();
	if (toPosDir.y < 0.1f) {
		m_toPos = m_toCameraPosOld;
	}
	else if (toPosDir.y > 0.9f) {
		m_toPos = m_toCameraPosOld;
	}
	//視点の計算
	m_position = m_target + m_toPos;
	//Pos.y += 50.0f;
	//ターゲットを少し高くする
	m_target.y += 100.0f;
	//メインカメラの注視点と視点を設定する。
	g_camera3D.SetTarget(m_target);
	g_camera3D.SetPosition(m_position);
	g_camera3D.Update();
}
void GameCamera::Draw()
{

}