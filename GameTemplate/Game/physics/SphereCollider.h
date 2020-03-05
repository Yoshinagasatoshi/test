/// <summary>
/// ���̃R���C�_�[
/// </summary>
#pragma once

#include "physics/ICollider.h"
class SphereCollider : public ICollider
{
public:
	/// <summary>
	/// �R���X�g���N�^�ƃf�X�g���N�^
	/// </summary>
	SphereCollider();
	~SphereCollider();
	/// <summary>
	/// ���̃R���C�_�[���쐬
	/// </summary>
	void Create(const float radius);
	btCollisionShape* GetBody() const override
	{
		return sphereShape;
	}
private:
	btSphereShape* sphereShape;
};