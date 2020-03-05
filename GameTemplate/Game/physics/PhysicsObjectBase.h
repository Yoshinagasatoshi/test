#pragma once
#include "ICollider.h"
/// <summary>
/// �����I�u�W�F�N�g�̃x�[�X�N���X�B
/// </summary>
//class ICollider;
class PhysicsObjectBase {
public:
	/// <summary>
	/// �S�[�X�g�I�u�W�F�N�g���J��
	/// <remarks>
	/// �����I�ȃ^�C�~���O�ŃI�u�W�F�N�g���폜�������Ƃ��Ɏg��
	/// </remarks>
	/// </summary> 
	virtual void Release() = 0;
private:
	/// <summary>
	/// �ÓI�����I�u�W�F�N�g�쐬�����̋��ʏ���
	/// </summary>
	virtual void CreateCommon(CVector3 pos, CQuaternion rot) = 0;
public:
	// �R���X�g���N�^
	PhysicsObjectBase()
	{

	}
	//�f�X�g���N�^
	virtual ~PhysicsObjectBase()
	{

	}
	/// <summary>
	/// �{�b�N�X�`��̃S�[�X�g�I�u�W�F�N�g���쐬
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="rot">��]</param>
	/// <param name="size">�T�C�Y</param>
	void CreateBox(
		CVector3 pos,
		CQuaternion rot,
		CVector3 size
	);
	/// <summary>
	/// �J�v�Z���`��̃S�[�X�g�I�u�W�F�N�g���쐬�B
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="rot">��]</param>
	/// <param name="radius">���a</param>
	/// <param name="height">����</param>
	void CreateCapsule(
		CVector3 pos,
		CQuaternion rot,
		float radius, 
		float height
	);
	/// <summary>
	/// ���̌`��̃S�[�X�g�I�u�W�F�N�g���쐬
	/// </summary>
	/// <param name="pos">���W</param>
	/// <param name="rot">��]</param>
	/// <param name="radius">���̔��a</param>
	void CreateSphere(
		CVector3 pos,
		CQuaternion rot, 
		float radius
	);
	//skinModel�N���X���g���ă��b�V�������
	void CreateMesh(CVector3 pos, CQuaternion rot, CVector3 scale , const SkinModel& skinModel);
protected:
	std::unique_ptr<ICollider> m_collider; //!�R���C�_�[
};