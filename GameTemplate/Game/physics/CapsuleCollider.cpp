/*!
 * @brief	カプセルコライダー。
 */

#include "stdafx.h"
#include "physics/CapsuleCollider.h"



/*!
	* @brief	デストラクタ。
	*/
CapsuleCollider::~CapsuleCollider()
{
	delete shape;
}
