/*!
* @brief	キャラクタコントローラー。
*/

#pragma once

#include "Physics/CapsuleCollider.h"
#include "Physics/RigidBody.h"
#include "Physics/SphereCollider.h"

/*!
* @brief	キャラクタコントローラー。
*/
class CharacterController {
public:
	CharacterController() {

	}
	~CharacterController()
	{
		RemoveRigidBoby();
	}
	/*!
		* @brief	初期化。
		*@param[in]	radius		カプセルコライダーの半径。
		*@param[in]	height		カプセルコライダーの高さ。
		*@param[in]	position	初期位置。
		*/
	void Init(float radius, float height, const CVector3& position);
	/*!
		* @brief	実行。
		*@param[in]	deltaTime		経過時間。単位は秒。
		*@param[in, out]	moveSpeed		移動速度。内部で重力加速が計算され、その結果がmoveSpeedに反映されます。
		*@return 移動後のキャラクターの座標。
		*/
	const CVector3& Execute(float deltaTime, CVector3& moveSpeed);
	/*!
		* @brief	座標を取得。
		*/
	const CVector3& GetPosition() const
	{
		return m_position;
	}
	/*!
		* @brief	座標を設定。
		*/
	void SetPosition(const CVector3& pos)
	{
		m_position = pos;
	}
	void SetAddPosition(const CVector3& vec)
	{
		m_addVector = vec;
	}
	/*!
		* @brief	ジャンプ中か判定
		*/
	bool IsJump() const
	{
		return m_isJump;
	}
	/*!
	* @brief	地面上にいるか判定。
	*/
	bool IsOnGround() const
	{
		return m_isOnGround;
	}
	/*!
	* @brief	コライダーを取得。
	*/
	CapsuleCollider* GetCollider()
	{
		return &m_collider;
	}
	/// <summary>
	/// スフィアコライダーを取得
	/// </summary>
	/// <returns></returns>
	SphereCollider* GetSphereCollider()
	{
		return &m_sphereCollider;
	}
	/*!
	* @brief	剛体を取得。
	*/
	RigidBody* GetRigidBody()
	{
		return &m_rigidBody;
	}
	/*!
	* @brief	剛体を物理エンジンから削除。。
	*/
	void RemoveRigidBoby();
private:
	CVector3 			m_position = CVector3::Zero();	//座標。
	bool 				m_isJump = false;				//ジャンプ中？
	bool				m_isOnGround = true;			//地面の上にいる？
	CapsuleCollider		m_collider;						//コライダー。
	SphereCollider		m_sphereCollider;				//球体コライダー
	float				m_radius = 0.0f;
	float				m_height = 0.0f;		
	RigidBody			m_rigidBody;					//剛体。
	CVector3			m_addVector = CVector3::Zero();
};
