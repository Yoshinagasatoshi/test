/*!
 * @brief	�ÓI�I�u�W�F�N�g�B
 */

#pragma once

#include "physics/MeshCollider.h"
#include "physics/RigidBody.h"
#include "physics/PhysicsObjectBase.h"

/*!
* @brief	�ÓI�����I�u�W�F�N�g
*/
class PhysicsStaticObject : public PhysicsObjectBase{
public:
	/*!
		* @brief	�R���X�g���N�^�B
		*/
	PhysicsStaticObject() {};
	/*!
		* @brief	�f�X�g���N�^�B
		*/
	~PhysicsStaticObject()
	{
		Release();
	}
	/// <summary>
	/// ���
	/// </summary>
	void Release() override final;
	/// <summary>
	/// ���W�Ɖ�]��ݒ�
	/// </summary>
	void SetPositionAndRotation(const CVector3& pos, const CQuaternion& rot)
	{
		m_rigidBody.SetPositionAndRotation(pos, rot);
	}
	/*!
		* @brief	���b�V���̐ÓI�I�u�W�F�N�g���쐬�B
		*@param[in]	skinModel	�X�L�����f���B
		*@param[in]	pos			���W�B
		*@param[in]	rot			��]�B
		*/
	void CreateMeshObject(SkinModel& skinModel, CVector3 pos, CQuaternion rot);
public:
	void CreateCommon(CVector3 pos,CQuaternion rot) override final;
private:
	MeshCollider m_meshCollider;		//!<���b�V���R���C�_�[�B
	RigidBody m_rigidBody;				//!<���́B
};
