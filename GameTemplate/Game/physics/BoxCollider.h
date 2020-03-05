/// <summary>
/// �{�b�N�X�R���C�_�[
/// </summary>
#pragma once

#include "physics/ICollider.h"

class BoxCollider : public ICollider
{
public:
	/// <summary>
	/// �R���X�g���N�^�ƃf�X�g���N�^
	/// </summary>
	BoxCollider();
	~BoxCollider();

	//�{�b�N�X�R���C�_�[���쐬
	void Create(const CVector3& size);
	btCollisionShape* GetBody() const override
	{
		return Boxshape;
	}
private:
	btBoxShape*		Boxshape;
};

