#pragma once
#include "gameObject/ysGameObject.h"
#include "GameData.h"
#include "graphics/RenderTarget.h"
#include "graphics/Sprite.h"
class UI;
class Player;
class Enemy;
class BackGround;
class GameCamera;
class Game : public IGameObject
{
public:
	enum EnRenderMode {
		enRenderMode_NORMAL,
		enRenderMode_Silhouette,
		enRenderMode_Num
	};
	//�R���X�g���N�^
	Game();
	//�f�X�g���N�^
	~Game();
	void Update();
	//�������牺�̏����̓N���X������Ĉϑ�����B
	void Draw();
	void ChangeRenderTarget(ID3D11DeviceContext* d3dDeviceContext, RenderTarget* renderTarget, D3D11_VIEWPORT* viewport);
	void ChangeRenderTarget(ID3D11DeviceContext* d3dDeviceContext, ID3D11RenderTargetView* renderTarget, ID3D11DepthStencilView* depthStensil, D3D11_VIEWPORT* viewport);
	void Render();
	void ForwardRender();
	void PostRender();
	//�J�����̏����J�[
	void InitCamera();
private:
	//�v���C���[
	Player* m_player = nullptr;
	Enemy* m_enemy = nullptr;
	BackGround* m_backGround = nullptr;
	GameCamera* m_gameCamera = nullptr;
	GameData* m_gamedata = nullptr;
	UI* m_ui;
	RenderTarget m_renderTarget;				//���C�������_�����O�^�[�Q�b�g
	Sprite m_copyMainRtToFrameBufferSprite;		//���C�������_�����O�ɕ`���ꂽ�G���t���[���o�b�t�@�ɃR�s�[���邽�߂̃X�v���C�g
	D3D11_VIEWPORT m_frameBufferViewports;		//�t���[���o�b�t�@�̃r���[�|�[�g
	ID3D11RenderTargetView* m_frameBufferRenderTargetView = nullptr;	//�t���[���o�b�t�@�̃����_�����O�^�[�Q�b�g�r���[
	ID3D11DepthStencilView* m_frameBufferDepthStencilView = nullptr;	//�t���[���o�b�t�@�̃f�v�X�X�e���V���r���[
	bool m_attack = false;
	bool m_isWireDraw = false;					//���C���[�t���[����\�����邩�ǂ����Btrue�ł��Ă���
};