/// <summary>
/// ���^�E�R���C�_�[
/// </summary>
#include "stdafx.h"
#include "physics/SphereCollider.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
SphereCollider::SphereCollider() :
	sphereShape(NULL)
{
}
/// <summary>
/// �f�X�g���N�^
/// </summary>
SphereCollider::~SphereCollider()
{
	delete sphereShape;
}
/// <summary>
/// ���̃R���C�_�[���쐬
/// </summary>
void SphereCollider::Create(const float radius)
{
	sphereShape = new btSphereShape(radius);
}