#pragma once
/// <summary>
/// �����_�����O�^�[�Q�b�g�N���X
/// </summary>
class RenderTarget 
{
public:
	RenderTarget();

	/// <summary>
	/// �����_�����O�^�[�Q�b�g�̍쐬�B
	/// �`��̏������ݐ�₼
	/// </summary>
	/// <param name="w">�e�N�X�`���̕�</param>
	/// <param name="h">�e�N�X�`���̍���</param>
	/// <param name="textFormat">�e�N�X�`���̃t�H�[�}�b�g</param>
	void Create(unsigned int w, unsigned int h, DXGI_FORMAT textFormat);

	/// <summary>
	/// �����_�����O�^�[�Q�b�g�ƃf�v�X�X�e���V���r���[�̃N���A
	/// </summary>
	/// <param name="clearColor"></param>
	void ClearRenderTarget(float* clearColor);

	/// <summary>
	/// �����_�����O�^�[�Q�b�g�r���[���擾�B
	/// </summary>
	ID3D11RenderTargetView* GetRenderTargetView()
	{
		return m_renderTargetView;
	}

	/// <summary>
	/// �f�v�X�X�e���V���r���[���擾
	/// </summary>
	/// <returns></returns>
	ID3D11DepthStencilView* GetDepthStensilView()
	{
		return m_depthStencilView;
	}

	/// <summary>
	/// �V�F�[�_�[���\�[�X�r���[���擾
	/// </summary>
	/// <returns>�����_�����O�^�[�Q�b�g��SRV</returns>
	ID3D11ShaderResourceView* GetRenderTargetSRV()
	{
		return m_RT_SRV;
	}
private:
	//�����_�����O�^�[�Q�b�g�֌W�̃����o�ϐ�
	ID3D11Texture2D* m_renderTargetTexture = nullptr;			//!�����_�����O�^�[�Q�b�g�ɏ������܂��e�N�X�`��
	ID3D11RenderTargetView* m_renderTargetView = nullptr;		//!�����_�����O�^�[�Q�b�g�r���[�B�������ݐ�����߂��B
	ID3D11ShaderResourceView* m_RT_SRV = nullptr;				//!�V�F�[�_�[���\�[�X�r���[�B�F���������ނ�B
	ID3D11Texture2D* m_depthStencilTexture = nullptr;			//!�f�v�X�X�e���V���r���[�Ɏg����texture
	ID3D11DepthStencilView* m_depthStencilView = nullptr;		//!�f�v�X�X�e���V���r���[�B�[�x�^�����B
};