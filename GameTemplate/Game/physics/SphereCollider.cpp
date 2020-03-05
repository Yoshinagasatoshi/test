/// <summary>
/// 旧タウコライダー
/// </summary>
#include "stdafx.h"
#include "physics/SphereCollider.h"

/// <summary>
/// コンストラクタ
/// </summary>
SphereCollider::SphereCollider() :
	sphereShape(NULL)
{
}
/// <summary>
/// デストラクタ
/// </summary>
SphereCollider::~SphereCollider()
{
	delete sphereShape;
}
/// <summary>
/// 球体コライダーを作成
/// </summary>
void SphereCollider::Create(const float radius)
{
	sphereShape = new btSphereShape(radius);
}