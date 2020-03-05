#include "stdafx.h"
#include "RenderTarget.h"

RenderTarget::RenderTarget()
{
	if (m_depthStencilView != nullptr) {
		m_depthStencilView->Release();
		m_depthStencilView = nullptr;
	}
	if (m_depthStencilTexture != nullptr) {
		m_depthStencilTexture->Release();
		m_depthStencilTexture = nullptr;
	}
	if (m_renderTargetView != nullptr) {
		m_renderTargetView->Release();
		m_renderTargetView = nullptr;
	}
	if (m_renderTargetTexture != nullptr) {
		m_renderTargetTexture->Release();
		m_renderTargetTexture = nullptr;
	}
	if (m_RT_SRV != nullptr) {
		m_RT_SRV->Release();
	}

}

void RenderTarget::Create(unsigned int w, unsigned int h, DXGI_FORMAT textFormat)
{
	//D3D�f�o�C�X���擾
	auto d3dDevice = g_graphicsEngine->GetD3DDevice();
	//1.�����_�����O�^�[�Q�b�g�ƂȂ�e�N�X�`�����쐬�B
	D3D11_TEXTURE2D_DESC texDesc = { 0 };
	{
		/// <summary>
		/// �e�N�X�`�����쐬���Ă���
		/// </summary>
		//�e�N�X�`���̉��̉𑜓x�B
		texDesc.Width = w;
		//�c
		texDesc.Height = h;
		//�~�b�v�}�b�v���x���B���l���傫���قǉ����̉�ʂ��ڂ₯��悤�ɂȂ�B
		//1�ł����B
		texDesc.MipLevels = 1;
		//�e�N�X�`���z��̃T�C�Y�B1�ł����B
		texDesc.ArraySize = 1;
		/// <summary>
		/// �e�N�X�`���̃t�H�[�}�b�g
		/// RGBA�̊e�v�f�ɉ�bit�U�蕪���č��v�l����
		/// �t�H�[�}�b�g�ɂ���B
		/// �����16bit*4��64bit�t�H�[�}�b�g�ɂ���
		/// </summary>
		texDesc.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
		//�e�N�X�`���̎g�p�p�r�����߂�B
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		//�}���`�T���v�����O(MSAA)�̐��B����4pix��
		//�[�x�̑傫���ɂ���ău�����h������������B
		//1�ł����B
		texDesc.SampleDesc.Count = 1;
		//�}���`�T���v�����O(MSAA)�̕i���B0�ł���
		texDesc.SampleDesc.Quality = 0;
		//�e�N�X�`���̓ǂݏ����̂����
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		//CPU����̃A�N�Z�X�̎d��
		texDesc.CPUAccessFlags = 0;
		//0�ł悢
		texDesc.MiscFlags = 0;
		//�e�N�X�`�����쐬�B
		d3dDevice->CreateTexture2D(&texDesc, nullptr, &m_renderTargetTexture);
	}
	//2.�����_�����O�^�[�Q�b�g�r���[�̍쐬
	{
		//�����_�����O�^�[�Q�b�g�r���[���쐬���邽�߂̃f�[�^��ݒ肷��B�B
		D3D11_RENDER_TARGET_VIEW_DESC viewDesc;
		//�t�H�[�}�b�g�̓e�N�X�`���Ɠ����ɂ���B
		viewDesc.Format = texDesc.Format;
		//2D�e�N�X�`�����������Ƃ��w�肷��B
		viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		//0�ł����B
		viewDesc.Texture2D.MipSlice = 0;
		//�����_�����O�^�[�Q�b�g�r���[�̍쐬�B
		d3dDevice->CreateRenderTargetView(m_renderTargetTexture, &viewDesc, &m_renderTargetView);
	}
	//3.�V�F�[�_�[���\�[�X�r���[�̍쐬
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		//�ӂ��[�܂��Ƃ̓e�N�X�`���Ɠ����ł悢
		srvDesc.Format = texDesc.Format;
		//�񎟌��e�N�X�`���Ƃ��Ĉ����B
		srvDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
		//�~�b�v�}�b�v���x���E�e�N�X�`���Ɠ����ł悢�B
		srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
		//0�ł����B
		srvDesc.Texture2D.MostDetailedMip = 0;
		//SRV�쐬
		d3dDevice->CreateShaderResourceView(m_renderTargetTexture, &srvDesc, &m_RT_SRV);
	}
	//4.�f�v�X�X�e���V���r���[�̍쐬
	D3D11_TEXTURE2D_DESC depthTexDesc = texDesc;
	{
		//�f�v�X�X�e���V���r���[�Ƀo�C���h(�֘A�t��)����B
		depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		//32bit���������_�̃f�v�X�X�e���V���o�b�t�@���쐬�B
		depthTexDesc.Format = DXGI_FORMAT_D32_FLOAT;
		//�f�v�X�X�e���V���e�N�X�`�����쐬����B
		d3dDevice->CreateTexture2D(&depthTexDesc, nullptr, &m_depthStencilTexture);
	}
	//5.�f�v�X�X�e���V���r���[�̍쐬
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		//�t�H�[�}�b�g�̓f�v�X�X�e���V���Ɠ����ɂ���
		depthStencilViewDesc.Format = depthTexDesc.Format;
		//2D�e�N�X�`�����������Ƃ��w�肷��B
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		//�~�b�v�}�b�v��0��ּ!
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		//������0
		depthStencilViewDesc.Flags = 0;
		//�f�v�X�X�e���V���r���[���쐬�B
		d3dDevice->CreateDepthStencilView(m_depthStencilTexture, &depthStencilViewDesc, &m_depthStencilView);
	}
}


void RenderTarget::ClearRenderTarget(float* clearColor)
{
	auto d3dDeviceContect = g_graphicsEngine->GetD3DDeviceContext();
	//�����_�[�^�[�Q�b�g���N���A
	d3dDeviceContect->ClearRenderTargetView(m_renderTargetView, clearColor);
	d3dDeviceContect->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH,1.0f,0);
}