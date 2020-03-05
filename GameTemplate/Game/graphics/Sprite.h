#pragma once
#include <algorithm>
#include "graphics/shader.h"
/// <summary>
/// ���݂̃X�v���C�g�N���X
/// <remarks>
/// �摜�𒣂�t���邱�Ƃ͂ł��邪���̂܂܂ł́A
/// pivot���Z�J���[�Ȃǂ�\�����邱�Ƃ��o���Ȃ��B
/// �v����
/// </remarks>
/// </summary>
class Sprite {
public:
	/// <summary>
	/// �R���X�g���N�^�B
	/// </summary>
	Sprite::Sprite();
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	Sprite::~Sprite();
	
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="textureFilePath">�e�N�X�`��</param>
	/// <param name="w">�摜�̕�</param>
	/// <param name="h">�摜�̍���</param>
	void Init(const wchar_t* textureFilePath, float w, float h);
	/// <summary>
	/// �e�N�X�`����SRV���w�肵�ď�����
	/// </summary>
	/// <param name="srv"></param>
	/// <param name="w"></param>
	/// <param name="h"></param>
	void Init(ID3D11ShaderResourceView* srv, float w, float h);
	/// <summary>
	/// ���������ʏ���
	/// </summary>
	/// <param name="w"></param>
	/// <param name="h"></param>
	void InitCommon(float w , float h);
	/// <summary>
	/// ���[���h�s����X�V�B
	/// </summary>
	/// <param name="trans">���s�ړ���</param>
	/// <param name="rot">��]</param>
	/// <param name="scale">�g�嗦</param>
	/// <param name="pivot"></param>
	/// �s�{�b�g...�摜�̊�_
	/// 0.5,0.5�ŉ摜�̐^��
	/// 0.0,0.0�ŉ摜�̍���
	/// 1.0,1.0�ŉ摜�̉E��
	void Update(const CVector3& trans, CQuaternion rot, CVector3 scale, CVector2 pivot = { 0.5f,0.5f });
	/// <summary>
	/// �`��B
	/// </summary>
	/// <param name="mView">�J�����s��</param>
	/// /// <param name="mView">�v���W�F�N�V�����s��</param>
	void Draw();
private:
	/// <summary>
	/// �V�F�[�_�[�����[�h�B
	/// </summary>
	void LoadShader();
	/// <summary>
	/// ���_�o�b�t�@������������B
	/// </summary>
	void InitVertexBuffer(float w, float h);
	/// <summary>
	/// �C���f�b�N�X�o�b�t�@���쐬�B
	/// </summary>
	void InitIndexBuffer();
	/// <summary>
	/// �T���v���X�e�[�g���쐬�B
	/// </summary>
	void InitSamplerState();
	/// <summary>
	/// �萔�o�b�t�@�̍쐬�B
	/// </summary>
	void InitConstantBuffer();
	/// <summary>
	/// �e�N�X�`�������\�h�B
	/// </summary>
	/// <param name="textureFIlePath">���[�h����e�N�X�`���̃t�@�C���p�X�B</param>
	void LoadTexture(const wchar_t* textureFIlePath);
private:
	struct ConstantBuffer {
		CMatrix WVP;  //WorldViewProjection(�s��)�̗�
		float alpha;
	};
	ID3D11Buffer* m_vertexBuffer = nullptr;	//VRAM��̒��_�o�b�t�@�ɃA�N�Z�X���邽�߂̃C���^�[�t�F�[�X�B
	ID3D11Buffer* m_indexBuffer = nullptr;		//VRAM��̃C���f�b�N�X�o�b�t�@�ɃA�N�Z�X���邽�߂̃C���^�[�t�F�[�X�B
	ID3D11Buffer* m_cb = nullptr;			//GPU���̒萔�o�b�t�@�ɃA�N�Z�X���邽�߂̃C���^�[�t�F�[�X�B
	ID3D11ShaderResourceView* m_texture = nullptr;	//�e�N�X�`���ɃA�N�Z�X���邽�߂̃C���^�[�t�F�[�X�B
	ID3D11SamplerState* m_samplerState = nullptr;	//�T���v���X�e�[�g�B
	Shader	m_vs;											//���_�V�F�[�_�[�B
	Shader	m_ps;											//�s�N�Z���V�F�[�_�[�B
	CMatrix m_world = CMatrix::Identity();					//���[���h�s��B
	CVector3 m_position = CVector3::Zero();		
	CQuaternion m_rotation = CQuaternion::Identity();
	CVector3 m_scale = CVector3::One();
	CVector2 m_size = CVector2::Zero();
	float m_alpha = 1.0f;

};