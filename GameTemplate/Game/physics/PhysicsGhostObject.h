/// <summary>
/// ゴーストオブジェクトクラス
/// すり抜ける必要のある当たり判定に使う。
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
	/// ゴーストオブジェクトを開放。
	/// </summary>
	void Release() override final;
	/// <summary>
	/// 引数で渡されたゴーストオブジェクトが自分かどうかの判定
	/// </summary>
	bool IsSelf(const btCollisionObject& ghost) const
	{
		return &ghost == &m_ghostObject;
	}
	/// <summary>
	/// 座標を設定
	/// </summary>
	void SetPosition(const CVector3& pos)
	{
		auto& btTrans = m_ghostObject.getWorldTransform();
		btVector3 btPos;
		pos.CopyTo(btPos);
		btTrans.setOrigin(btPos);
	}
	/// <summary>
	/// 回転を設定
	/// </summary>
	void SetRotation(const CQuaternion& rot)
	{
		auto& btTrans = m_ghostObject.getWorldTransform();
		btQuaternion btrot;
		rot.CopyTo(btrot);
	}
private:
	/// <summary>
	/// ゴースト作成処理の情通処理
	/// </summary>
	void CreateCommon(CVector3 pos, CQuaternion rot) override;

	bool			m_isRegistPhysicsWorld = false;//!物理ワールドに登録しているかどうかのフラグ。
	btGhostObject	m_ghostObject;	//!<ゴーストオブジェクト>
};