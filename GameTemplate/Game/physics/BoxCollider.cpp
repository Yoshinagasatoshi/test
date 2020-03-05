#include "stdafx.h"
#include "physics/BoxCollider.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
BoxCollider::BoxCollider() :
	Boxshape(NULL)
{

}
/// <summary>
/// �f�X�g���N�^
/// </summary>
BoxCollider::~BoxCollider()
{
	delete Boxshape;
}
/// <summary>
/// �{�b�N�X�R���C�_�[���쐬
/// </summary>
void BoxCollider::Create(const CVector3& size)
{
	Boxshape = new btBoxShape(btVector3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f));
}
