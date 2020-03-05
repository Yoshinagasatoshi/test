#pragma once
#include "character/CharacterController.h"
#include "DebugWireframe.h"
class RigidBody;
//class CharacterController;

class PhysicsWorld
{
	//btDefaultCollisionConfiguration*		collisionConfig = nullptr;
	//btCollisionDispatcher*					collisionDispatcher = nullptr;	//!<�Փˉ��������B
	//btBroadphaseInterface*					overlappingPairCache = nullptr;	//!<�u���[�h�t�F�[�Y�B�Փ˔���̎}�؂�B
	//btSequentialImpulseConstraintSolver*	constraintSolver = nullptr;		//!<�R���X�g���C���g�\���o�[�B�S�������̉��������B
	//btDiscreteDynamicsWorld*				dynamicWorld = nullptr;			//!<���[���h�B

	std::unique_ptr<btDefaultCollisionConfiguration>	m_collisionConfig;
	std::unique_ptr<btCollisionDispatcher>				m_collisionDispatcher;//!<�Փˉ��������B
	std::unique_ptr<btBroadphaseInterface>				m_overlappingPairCache;//!<�u���[�h�t�F�[�Y�B�Փ˔���̎}�؂�B
	std::unique_ptr<btSequentialImpulseConstraintSolver>m_constraintSolver;//!<�R���X�g���C���g�\���o�[�B�S�������̉��������B
	std::unique_ptr<btDiscreteDynamicsWorld>			m_dynamicWorld;//!<���[���h�B

	DebugWireframe m_dw;
public:
	~PhysicsWorld();
	void Init();
	void Update();
	void Release();
	
	/// <summary>
	/// �d�͂�ݒ�
	/// </summary>
	void SetGravity(CVector3 g)
	{
		m_dynamicWorld->setGravity(btVector3(g.x, g.y, g.z));
	}

	//PhysicsWorld& GetInstance()
	//{
	//	return m_PhysicsWorld;
	//}
	/*!
	* @brief	�_�C�i�~�b�N���[���h���擾�B
	*/
	btDiscreteDynamicsWorld* GetDynamicWorld()
	{
		return m_dynamicWorld.get();
	}
	/*!
	* @brief	���̂�o�^�B
	*/
	void AddRigidBody(RigidBody& rb);
	/*!
	* @brief	���̂�j���B
	*/
	void RemoveRigidBody(RigidBody& rb);
	/// <summary>
	/// �R���W�����I�u�W�F�N�g�����[���h�ɓo�^
	/// </summary>
	/// <param name="colliObj">�R���W�����I�u�W�F�N�g</param>
	void AddCollisionObject(btCollisionObject& colliObj)
	{
		m_dynamicWorld->addCollisionObject(&colliObj);
	}
	/// <summary>
	/// �R���W�����I�u�W�F�N�g�����[���h����폜����B
	/// </summary>
	/// <param name="colliObj"></param>
	void RemoveCollisionObject(btCollisionObject& colliObj)
	{
		m_dynamicWorld->removeCollisionObject(&colliObj);
	}

	void ConvexSweepTest(
		const btConvexShape* castShape,
		const btTransform& convexFromWorld,
		const btTransform& convexToWorld,
		btCollisionWorld::ConvexResultCallback& resultCallback,
		btScalar allowedCcdPenetration = 0.0f
	)
	{
		m_dynamicWorld->convexSweepTest(castShape, convexFromWorld, convexToWorld, resultCallback, allowedCcdPenetration);
	}
	void ContactText(
		btCollisionObject* colObj, 
		btCollisionWorld::ContactResultCallback& resultCallback
	)
	{
		m_dynamicWorld->contactTest(colObj, resultCallback);
	}
	//��1������btCollision*
	void ContactTest(
		btCollisionObject* colObj,
		std::function<void(const btCollisionObject& contactCollisionObject)> cb
	);
	//��1������RigidBody&
	void ContactTest(
		RigidBody& rb,
		std::function<void(const btCollisionObject& contactCollisionObject)> cb
	);

	//��1������characon&
	void ContactTest(
		CharacterController& charaCon,
		std::function<void(const btCollisionObject& contactCollisionObject)> cb
	);
	void DebugDraw();
}; 
extern PhysicsWorld g_physics;