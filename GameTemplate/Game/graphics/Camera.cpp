#include "stdafx.h"
#include "Camera.h"

Camera g_camera3D;		//3Dカメラ。
Camera g_camera2D;		//2Dカメラ。

void Camera::Update()
{
	
	//ビュー行列を計算。
	m_viewMatrix.MakeLookAt(
		m_position,
		m_target,
		m_up
	);
	//ビュー行列の逆行列を計算。
	//これでカメラ行列になる
	m_viewMatrixInv.Inverse(m_viewMatrix);

	m_forward.Set(	
		m_viewMatrixInv.m[2][0],
		m_viewMatrixInv.m[2][1],
		m_viewMatrixInv.m[2][2]
	);
	m_right.Set(	
		m_viewMatrixInv.m[0][0],
		m_viewMatrixInv.m[0][1],
		m_viewMatrixInv.m[0][2]
	);
	//プロジェクション行列を計算。
	m_projMatrix.MakeProjectionMatrix(
		m_viewAngle,					//画角。
		FRAME_BUFFER_W / FRAME_BUFFER_H,	//アスペクト比。
		m_near,
		m_far
	);
}