#include "stdafx.h"
#include "physics/BoxCollider.h"

/// <summary>
/// コンストラクタ
/// </summary>
BoxCollider::BoxCollider() :
	Boxshape(NULL)
{

}
/// <summary>
/// デストラクタ
/// </summary>
BoxCollider::~BoxCollider()
{
	delete Boxshape;
}
/// <summary>
/// ボックスコライダーを作成
/// </summary>
void BoxCollider::Create(const CVector3& size)
{
	Boxshape = new btBoxShape(btVector3(size.x * 0.5f, size.y * 0.5f, size.z * 0.5f));
}
