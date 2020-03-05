#include "stdafx.h"
#include "ysGameObjectManager.h"
//ysGameObjectManager�N���X�̃C���X�^���X�B
ysGameObjectManager g_goMgr;
ysGameObjectManager::EffectTool g_Effect;
	void ysGameObjectManager::Start()
	{
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->StartWrapper();
			}
		}
	}

	//void ysGameObjectManager::Update()
	//{
	//	for (GameObjectList objList : m_gameObjectListArray) {
	//		for (IGameObject* obj : objList) {
	//			obj->UpdateWrapper();
	//		}
	//	}
	//}

	void ysGameObjectManager::PostUpdate()
	{
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->PostUpdateWrapper();
			}
		}
	}

	void ysGameObjectManager::PreUpdate()
	{
		for (GameObjectList objList : m_gameObjectListArray) {
			for (IGameObject* obj : objList) {
				obj->PreUpdateWrapper();
			}
		}
	}

	//void ysGameObjectManager::Draw()
	//{
	//	for (GameObjectList objList : m_gameObjectListArray) {
	//		for (IGameObject* obj : objList) {
	//			obj->DrawWrapper();
	//		}
	//	}
	//}

	void ysGameObjectManager::Update()
	{
		for (auto go : IGameObjectList) {
			go->Update();
			}
		//�S�ẴQ�[���I�u�W�F�N�g��1�t���[�����̏������I����Ă���A�폜����B
		for (auto it = IGameObjectList.begin(); it != IGameObjectList.end();) {
			if ((*it)->isRequestDelete()) {
				//�폜���N�G�X�g���󂯂Ă���̂ō폜
				delete* it;
				it = IGameObjectList.erase(it);
			}
			else {
				//���N�G�X�g���󂯂Ă��Ȃ�����
				it++;
			}
		}
	}

	void ysGameObjectManager::Draw()
	{
		for (auto go : IGameObjectList) {
			go->Draw();
		}
	}

	void ysGameObjectManager::PostRender()
	{
		for (auto go : IGameObjectList) {
			go->PostRender();
		}
	}
	void ysGameObjectManager::Render()
	{
		for (auto go : IGameObjectList) {
			go->Render();
		}
	}

	void ysGameObjectManager::Execute()
	{
	//	//�o�^����Ă���Q�[���I�u�W�F�N�g�̍X�V�������ĂԁB
	//	for (auto go : IGameObjectList) {
	//		go->Update();
	//		go->Draw();
	//	}
	}

	void ysGameObjectManager::Init(int gameObjectPrioMax)
	{
		IGameObjectList.reserve(gameObjectPrioMax);
		m_gameObjectListArray.resize(gameObjectPrioMax);
		m_deleteObjectArray[0].resize(gameObjectPrioMax);
		m_deleteObjectArray[1].resize(gameObjectPrioMax);
	}

	void ysGameObjectManager::InitEffekseer()
	{
		//�����_���[��������
		g_Effect.m_effekseerRenderer = EffekseerRendererDX11::Renderer::Create(
			g_graphicsEngine->GetD3DDevice(),
			g_graphicsEngine->GetD3DDeviceContext(),
			20000									//�|���̍ő吔�E
		);
		//�G�t�F�N�g�}�l�[�W����������
		g_Effect.m_effekseerManager = Effekseer::Manager::Create(10000);

		//�`��p�C���X�^���X����`��@�\��ݒ�
		g_Effect.m_effekseerManager->SetSpriteRenderer(g_Effect.m_effekseerRenderer->CreateSpriteRenderer());
		g_Effect.m_effekseerManager->SetRibbonRenderer(g_Effect.m_effekseerRenderer->CreateRibbonRenderer());
		g_Effect.m_effekseerManager->SetRingRenderer(g_Effect.m_effekseerRenderer->CreateRingRenderer());
		g_Effect.m_effekseerManager->SetTrackRenderer(g_Effect.m_effekseerRenderer->CreateTrackRenderer());
		g_Effect.m_effekseerManager->SetModelRenderer(g_Effect.m_effekseerRenderer->CreateModelRenderer());

		//�C���X�^���X����e�N�X�`���̓ǂݍ��݋@�\��ݒ�
		// �Ǝ��g���\�A���݂̓t�@�C������ǂݍ���ł���B
		g_Effect.m_effekseerManager->SetTextureLoader(g_Effect.m_effekseerRenderer->CreateTextureLoader());
		g_Effect.m_effekseerManager->SetModelLoader(g_Effect.m_effekseerRenderer->CreateModelLoader());
	}