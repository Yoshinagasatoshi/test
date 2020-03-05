#include "stdafx.h"
#include "physics/RigidBody.h"
#include "physics/ICollider.h"

using namespace std;

RigidBody::~RigidBody()
{
	Release();
}
void RigidBody::Release()
{
	//旧コード。すべてコメントアウトしている。
	/*delete rigidBody;
	delete myMotionState;
	rigidBody = nullptr;
	myMotionState = nullptr;*/
}
/// <summary>
/// 剛体を作る。
/// </summary>
/// <param name="rbInfo"></param>
void RigidBody::Create(RigidBodyInfo& rbInfo)
{

	Release();
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(rbInfo.pos.x, rbInfo.pos.y, rbInfo.pos.z));
	transform.setRotation(btQuaternion(rbInfo.rot.x, rbInfo.rot.y, rbInfo.rot.z, rbInfo.rot.w));
	m_myMotionState = make_unique<btDefaultMotionState>();
	m_myMotionState->setWorldTransform(transform);
	//btVector3 btLocalInteria;
	//rbInfo.localInteria.CopyTo(btLocalInteria);
	btRigidBody::btRigidBodyConstructionInfo btRbInfo(rbInfo.mass, m_myMotionState.get(), rbInfo.collider->GetBody(), btVector3(0, 0, 0));
	//剛体を作成。
	m_rigidBody = make_unique<btRigidBody>(btRbInfo);
}
