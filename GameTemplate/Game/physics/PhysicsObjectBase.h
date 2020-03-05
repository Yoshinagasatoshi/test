#pragma once
#include "ICollider.h"
/// <summary>
/// 物理オブジェクトのベースクラス。
/// </summary>
//class ICollider;
class PhysicsObjectBase {
public:
	/// <summary>
	/// ゴーストオブジェクトを開放
	/// <remarks>
	/// 明示的なタイミングでオブジェクトを削除したいときに使う
	/// </remarks>
	/// </summary> 
	virtual void Release() = 0;
private:
	/// <summary>
	/// 静的物理オブジェクト作成処理の共通処理
	/// </summary>
	virtual void CreateCommon(CVector3 pos, CQuaternion rot) = 0;
public:
	// コンストラクタ
	PhysicsObjectBase()
	{

	}
	//デストラクタ
	virtual ~PhysicsObjectBase()
	{

	}
	/// <summary>
	/// ボックス形状のゴーストオブジェクトを作成
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="rot">回転</param>
	/// <param name="size">サイズ</param>
	void CreateBox(
		CVector3 pos,
		CQuaternion rot,
		CVector3 size
	);
	/// <summary>
	/// カプセル形状のゴーストオブジェクトを作成。
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="rot">回転</param>
	/// <param name="radius">半径</param>
	/// <param name="height">高さ</param>
	void CreateCapsule(
		CVector3 pos,
		CQuaternion rot,
		float radius, 
		float height
	);
	/// <summary>
	/// 球体形状のゴーストオブジェクトを作成
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="rot">回転</param>
	/// <param name="radius">球体半径</param>
	void CreateSphere(
		CVector3 pos,
		CQuaternion rot, 
		float radius
	);
	//skinModelクラスを使ってメッシュを作る
	void CreateMesh(CVector3 pos, CQuaternion rot, CVector3 scale , const SkinModel& skinModel);
protected:
	std::unique_ptr<ICollider> m_collider; //!コライダー
};