#include "stdafx.h"
#include "ysGameObjectManager.h"
//ysGameObjectManagerクラスのインスタンス。
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
		//全てのゲームオブジェクトの1フレーム分の処理が終わってから、削除する。
		for (auto it = IGameObjectList.begin(); it != IGameObjectList.end();) {
			if ((*it)->isRequestDelete()) {
				//削除リクエストを受けているので削除
				delete* it;
				it = IGameObjectList.erase(it);
			}
			else {
				//リクエストを受けていないため
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
	//	//登録されているゲームオブジェクトの更新処理を呼ぶ。
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
		//レンダラーを初期化
		g_Effect.m_effekseerRenderer = EffekseerRendererDX11::Renderer::Create(
			g_graphicsEngine->GetD3DDevice(),
			g_graphicsEngine->GetD3DDeviceContext(),
			20000									//板ポリの最大数・
		);
		//エフェクトマネージャを初期化
		g_Effect.m_effekseerManager = Effekseer::Manager::Create(10000);

		//描画用インスタンスから描画機能を設定
		g_Effect.m_effekseerManager->SetSpriteRenderer(g_Effect.m_effekseerRenderer->CreateSpriteRenderer());
		g_Effect.m_effekseerManager->SetRibbonRenderer(g_Effect.m_effekseerRenderer->CreateRibbonRenderer());
		g_Effect.m_effekseerManager->SetRingRenderer(g_Effect.m_effekseerRenderer->CreateRingRenderer());
		g_Effect.m_effekseerManager->SetTrackRenderer(g_Effect.m_effekseerRenderer->CreateTrackRenderer());
		g_Effect.m_effekseerManager->SetModelRenderer(g_Effect.m_effekseerRenderer->CreateModelRenderer());

		//インスタンスからテクスチャの読み込み機能を設定
		// 独自拡張可能、現在はファイルから読み込んでいる。
		g_Effect.m_effekseerManager->SetTextureLoader(g_Effect.m_effekseerRenderer->CreateTextureLoader());
		g_Effect.m_effekseerManager->SetModelLoader(g_Effect.m_effekseerRenderer->CreateModelLoader());
	}