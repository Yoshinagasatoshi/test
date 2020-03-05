/*!
 * @brief	静的オブジェクト。
 */

#pragma once

#include "physics/MeshCollider.h"
#include "physics/RigidBody.h"
#include "physics/PhysicsObjectBase.h"

/*!
* @brief	静的物理オブジェクト
*/
class PhysicsStaticObject : public PhysicsObjectBase{
public:
	/*!
		* @brief	コンストラクタ。
		*/
	PhysicsStaticObject() {};
	/*!
		* @brief	デストラクタ。
		*/
	~PhysicsStaticObject()
	{
		Release();
	}
	/// <summary>
	/// 解放
	/// </summary>
	void Release() override final;
	/// <summary>
	/// 座標と回転を設定
	/// </summary>
	void SetPositionAndRotation(const CVector3& pos, const CQuaternion& rot)
	{
		m_rigidBody.SetPositionAndRotation(pos, rot);
	}
	/*!
		* @brief	メッシュの静的オブジェクトを作成。
		*@param[in]	skinModel	スキンモデル。
		*@param[in]	pos			座標。
		*@param[in]	rot			回転。
		*/
	void CreateMeshObject(SkinModel& skinModel, CVector3 pos, CQuaternion rot);
public:
	void CreateCommon(CVector3 pos,CQuaternion rot) override final;
private:
	MeshCollider m_meshCollider;		//!<メッシュコライダー。
	RigidBody m_rigidBody;				//!<剛体。
};
