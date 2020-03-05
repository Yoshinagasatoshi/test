/// <summary>
/// 球体コライダー
/// </summary>
#pragma once

#include "physics/ICollider.h"
class SphereCollider : public ICollider
{
public:
	/// <summary>
	/// コンストラクタとデストラクタ
	/// </summary>
	SphereCollider();
	~SphereCollider();
	/// <summary>
	/// 球体コライダーを作成
	/// </summary>
	void Create(const float radius);
	btCollisionShape* GetBody() const override
	{
		return sphereShape;
	}
private:
	btSphereShape* sphereShape;
};