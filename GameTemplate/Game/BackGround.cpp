#include "stdafx.h"
#include "BackGround.h"

BackGround::BackGround()
{
	//cmoファイルの読み込み。
	m_model.Init(L"Assets/modelData/musou_stage.cmo");
	//m_physicsStaticObject.CreateCommon(m_position, m_rotation);
	m_physicsStaticObject.CreateMeshObject(m_model, m_position, m_rotation);
}
BackGround::~BackGround()
{

}
void BackGround::Update()
{
	//ワールド行列の更新。
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	
}
void BackGround::Draw()
{
	m_model.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}