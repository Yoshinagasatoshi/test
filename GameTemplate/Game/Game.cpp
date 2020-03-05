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
//�R���X�g���N�^���Ă΂��ƃ��x���ŃL������\��������悤�ɂ��Ă���
Game::Game()
{
	InitCamera();
	//�v���C���[
	m_player = g_goMgr.NewGameObject<Player>("Player");
	m_backGround = g_goMgr.NewGameObject<BackGround>("BackGround");
	m_gameCamera = g_goMgr.NewGameObject<GameCamera>("GameCamera");
	m_gamedata = g_goMgr.NewGameObject<GameData>("GameData");
	//m_ui = g_goMgr.NewGameObject<UI>("UI");
	m_gameCamera->SetPlayerInfo(m_player);
	m_gamedata->SetPlayerInfo(m_player);
	//���x���Ń��f�����o���B
	Level level;
	level.Init(L"Assets/level/musou_honkakustage.tkl",
		[&](const LevelObjectData& objdata) {
			//���y
			if (wcscmp(objdata.name, L"asigaru") == 0) {
				//�C���X�^���X�̍쐬
				m_enemy = g_goMgr.NewGameObject<Enemy_asigaru>("Enemy_asigaru");
				m_enemy->SetPos(objdata.position);
				//enemy->SetRot(objdata.rotation);
				m_enemy->SetPlayerInfo(m_player);
				m_enemy->SetGameCameraInfo(m_gameCamera);
				//�ϒ��z��Ɂ��̃C���X�^���X��ǉ�
				return true;
			}
			if (wcscmp(objdata.name, L"enemy_busyo") == 0) {
				//�C���X�^���X�̍쐬
				m_enemy = g_goMgr.NewGameObject<Enemy_Busyo>("Enemy_busyo");
				m_enemy->SetPos(objdata.position);
				m_enemy->SetPlayerInfo(m_player);
				return true;
			}
		});
	//���C���ƂȂ郌���_�����O�^�[�Q�b�g
	m_renderTarget.Create(FRAME_BUFFER_W, FRAME_BUFFER_H, DXGI_FORMAT_R16G16B16A16_UNORM);

	//���ɕ`���ꂽ�G��
	//�t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g�̏���������
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
	/// render�֐��̏�����Draw�֐��ɏ����Ă�������
	/// �I�[�o�[�t���[���N�����̂�Render�Ɉϑ������B
	/// </summary>
}




/// <summary>
/// �������牺�A�ǂ����̃N���X�Ɉϑ��������B�B�B
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
	//�����_�����O�^�[�Q�b�g�̐؂�ւ�
	d3dDeviceContext->OMSetRenderTargets(1, rtTbl, depthStensil);
	if (viewport != nullptr) {
		//�r���[�|�[�g���w�肳��Ă�����A�r���[�|�[�g���ύX����B
		d3dDeviceContext->RSSetViewports(1, viewport);
	}
}

void Game::Render()
{
	//�`��J�n
	g_graphicsEngine->BegineRender();
	//�t���[���o�b�t�@�̃����_�����O�^�[�Q�b�g���o�b�N�A�b�v����B
	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	d3dDeviceContext->OMGetRenderTargets(
		1,
		&m_frameBufferRenderTargetView,
		&m_frameBufferDepthStencilView
	);
	//�r���[�|�[�g���o�b�N�A�b�v������Ă���
	unsigned int numViewport = 1;
	d3dDeviceContext->RSGetViewports(&numViewport, &m_frameBufferViewports);

	ForwardRender();

	PostRender();

	g_graphicsEngine->EndRender();
}

void Game::ForwardRender()
{
	//�����_�����O�^�[�Q�b�g�����C���ɕύX����B
	ID3D11DeviceContext* d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	ChangeRenderTarget(d3dDeviceContext, &m_renderTarget, &m_frameBufferViewports);

	//�����_�����O�^�[�Q�b�g���N���A�B
	float clearColor[4] = { 0.5f, 0.5f, 0.5f, 1.0f }; //red,green,blue,alpha
	m_renderTarget.ClearRenderTarget(clearColor);

	g_goMgr.Draw();
	//�G�t�F�N�g�͕s�����I�u�W�F�N�g��`�悵����ŕ`�悷��B
	g_Effect.m_effekseerRenderer->BeginRendering();
	g_Effect.m_effekseerManager->Draw();
	g_Effect.m_effekseerRenderer->EndRendering();
	if (m_isWireDraw) {
		g_physics.DebugDraw();
	}
}

void Game::PostRender()
{
	//�����_�����O�^�[�Q�b�g���t���[���o�b�t�@�ɖ߂��B
	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();
	
	ChangeRenderTarget(
		d3dDeviceContext,
		m_frameBufferRenderTargetView,
		m_frameBufferDepthStencilView,
		&m_frameBufferViewports
	);
	//�h���[
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
	//-600�����傤�ǂ�������
	g_camera2D.SetPosition({ 0.0f, 0.0f, camera2Dpos_z });
	g_camera2D.SetTarget(CVector3::Zero());
	g_camera2D.Update();
}

//�R�����g�Ƃ��A�����Ƃ��������B