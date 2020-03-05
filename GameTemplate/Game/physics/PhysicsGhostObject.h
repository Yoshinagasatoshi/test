/// <summary>
/// �S�[�X�g�I�u�W�F�N�g�N���X
/// ���蔲����K�v�̂��铖���蔻��Ɏg���B
/// </summary>
#pragma once
#include "physics/PhysicsObjectBase.h"

class PhysicsGhostObject : public PhysicsObjectBase{
public:
	~PhysicsGhostObject()
	{
		Release();
	}
	/// <summary>
	/// �S�[�X�g�I�u�W�F�N�g���J���B
	/// </summary>
	void Release() override final;
	/// <summary>
	/// �����œn���ꂽ�S�[�X�g�I�u�W�F�N�g���������ǂ����̔���
	/// </summary>
	bool IsSelf(const btCollisionObject& ghost) const
	{
		return &ghost == &m_ghostObject;
	}
	/// <summary>
	/// ���W��ݒ�
	/// </summary>
	void SetPosition(const CVector3& pos)
	{
		auto& btTrans = m_ghostObject.getWorldTransform();
		btVector3 btPos;
		pos.CopyTo(btPos);
		btTrans.setOrigin(btPos);
	}
	/// <summary>
	/// ��]��ݒ�
	/// </summary>
	void SetRotation(const CQuaternion& rot)
	{
		auto& btTrans = m_ghostObject.getWorldTransform();
		btQuaternion btrot;
		rot.CopyTo(btrot);
	}
private:
	/// <summary>
	/// �S�[�X�g�쐬�����̏�ʏ���
	/// </summary>
	void CreateCommon(CVector3 pos, CQuaternion rot) override;

	bool			m_isRegistPhysicsWorld = false;//!�������[���h�ɓo�^���Ă��邩�ǂ����̃t���O�B
	btGhostObject	m_ghostObject;	//!<�S�[�X�g�I�u�W�F�N�g>
};