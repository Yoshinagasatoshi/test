#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "Enemy.h"
#include "Enemy_asigaru.h"
#include "Enemy_Busyo.h"
#include "BackGround.h"
#include "GameCamera.h"
#include "level/Level.h"
#include "GameData.h"
#include "UI.h"
#include "gameObject/ysGameObjectManager.h"
//コンストラクタが呼ばれるとレベルでキャラを表示させるようにしている
Game::Game()
{
	InitCamera();
	//プレイヤー
	m_player = g_goMgr.NewGameObject<Player>("Player");
	m_backGround = g_goMgr.NewGameObject<BackGround>("BackGround");
	m_gameCamera = g_goMgr.NewGameObject<GameCamera>("GameCamera");
	m_gamedata = g_goMgr.NewGameObject<GameData>("GameData");
	//m_ui = g_goMgr.NewGameObject<UI>("UI");
	m_gameCamera->SetPlayerInfo(m_player);
	m_gamedata->SetPlayerInfo(m_player);
	//レベルでモデルを出す。
	Level level;
	level.Init(L"Assets/level/musou_honkakustage.tkl",
		[&](const LevelObjectData& objdata) {
			//足軽
			if (wcscmp(objdata.name, L"asigaru") == 0) {
				//インスタンスの作成
				m_enemy = g_goMgr.NewGameObject<Enemy_asigaru>("Enemy_asigaru");
				m_enemy->SetPos(objdata.position);
				//enemy->SetRot(objdata.rotation);
				m_enemy->SetPlayerInfo(m_player);
				m_enemy->SetGameCameraInfo(m_gameCamera);
				//可変長配列に↑のインスタンスを追加
				return true;
			}
			if (wcscmp(objdata.name, L"enemy_busyo") == 0) {
				//インスタンスの作成
				m_enemy = g_goMgr.NewGameObject<Enemy_Busyo>("Enemy_busyo");
				m_enemy->SetPos(objdata.position);
				m_enemy->SetPlayerInfo(m_player);
				return true;
			}
		});
	//メインとなるレンダリングターゲット
	m_renderTarget.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, DXGI_FORMAT_R16G16B16A16_UNORM);

	//↑に描かれた絵を
	//フレームバッファにコピーするためのスプライトの初期化する
	m_copyMainRtToFrameBufferSprite.Init(
		m_renderTarget.GetRenderTargetSRV(),
		FRAME_BUFFER_W,
		FRAME_BUFFER_H
	);
}

Game::~Game()
{
	g_goMgr.DeleteGOObject(m_player);
	g_goMgr.DeleteGOObject(m_backGround);
}

void Game::Update()
{
	if (g_pad->IsTrigger(enButtonUp)) {
		if (m_isWireDraw) {
			m_isWireDraw = false;
		}
		else {
			m_isWireDraw = true;
		}
	}
}


void Game::Draw()
{
	/// <summary>
	/// render関数の処理をDraw関数に書いていたけど
	/// オーバーフローが起きたのでRenderに委託した。
	/// </summary>
}




/// <summary>
/// ここから下、どっかのクラスに委託したい。。。
/// </summary>
 
void Game::ChangeRenderTarget(ID3D11DeviceContext* d3dDeviceContext, RenderTarget* renderTarget, D3D11_VIEWPORT* viewport)
{
	ChangeRenderTarget(
		d3dDeviceContext,
		renderTarget->GetRenderTargetView(),
		renderTarget->GetDepthStensilView(),
		viewport
	);
}

void Game::ChangeRenderTarget(ID3D11DeviceContext* d3dDeviceContext,ID3D11RenderTargetView* renderTarget,ID3D11DepthStencilView* depthStensil,D3D11_VIEWPORT* viewport)
{
	ID3D11RenderTargetView* rtTbl[] = {
		renderTarget
	};
	//レンダリングターゲットの切り替え
	d3dDeviceContext->OMSetRenderTargets(1, rtTbl, depthStensil);
	if (viewport != nullptr) {
		//ビューポートが指定されていたら、ビューポートも変更する。
		d3dDeviceContext->RSSetViewports(1, viewport);
	}
}

void Game::Render()
{
	//描画開始
	g_graphicsEngine->BegineRender();
	//フレームバッファのレンダリングターゲットをバックアップする。
	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	d3dDeviceContext->OMGetRenderTargets(
		1,
		&m_frameBufferRenderTargetView,
		&m_frameBufferDepthStencilView
	);
	//ビューポートもバックアップを取っておく
	unsigned int numViewport = 1;
	d3dDeviceContext->RSGetViewports(&numViewport, &m_frameBufferViewports);

	ForwardRender();

	PostRender();

	g_graphicsEngine->EndRender();
}

void Game::ForwardRender()
{
	//レンダリングターゲットをメインに変更する。
	ID3D11DeviceContext* d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	ChangeRenderTarget(d3dDeviceContext, &m_renderTarget, &m_frameBufferViewports);

	//レンダリングターゲットをクリア。
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; //red,green,blue,alpha
	m_renderTarget.ClearRenderTarget(clearColor);

	g_goMgr.Draw();
	//エフェクトは不透明オブジェクトを描画した後で描画する。
	g_Effect.m_effekseerRenderer->BeginRendering();
	g_Effect.m_effekseerManager->Draw();
	g_Effect.m_effekseerRenderer->EndRendering();
	if (m_isWireDraw) {
		g_physics.DebugDraw();
	}
}

void Game::PostRender()
{
	//レンダリングターゲットをフレームバッファに戻す。
	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	
	ChangeRenderTarget(
		d3dDeviceContext,
		m_frameBufferRenderTargetView,
		m_frameBufferDepthStencilView,
		&m_frameBufferViewports
	);
	//ドロー
	m_copyMainRtToFrameBufferSprite.Draw();

	m_frameBufferRenderTargetView->Release();
	m_frameBufferDepthStencilView->Release();

}

void Game::InitCamera()
{	
	const float camera2Dpos_z = -600.0f;

	g_camera3D.Update();
	g_camera2D.SetUpdateProjMatrixFunc(Camera::enUpdateProjMatrixFunc_Ortho);
	g_camera2D.SetWidth(FRAME_BUFFER_W);
	g_camera2D.SetHeight(FRAME_BUFFER_H);
	//-600がちょうどいい感じ
	g_camera2D.SetPosition({ 0.0f, 0.0f, camera2Dpos_z });
	g_camera2D.SetTarget(CVector3::Zero());
	g_camera2D.Update();
}

//コメントとか、処理とかを書く。