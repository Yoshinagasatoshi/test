/// <summary>
/// ボックスコライダー
/// </summary>
#pragma once

#include "physics/ICollider.h"

class BoxCollider : public ICollider
{
public:
	/// <summary>
	/// コンストラクタとデストラクタ
	/// </summary>
	BoxCollider();
	~BoxCollider();

	//ボックスコライダーを作成
	void Create(const CVector3& size);
	btCollisionShape* GetBody() const override
	{
		return Boxshape;
	}
private:
	btBoxShape*		Boxshape;
};

