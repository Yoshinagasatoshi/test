#include "stdafx.h"
#include "DebugWireframe.h"

DebugWireframe::DebugWireframe()
{

}

DebugWireframe::~DebugWireframe()
{

}

void DebugWireframe::Prepare()
{
	///���_�o�b�t�@ 
	///0�\����
	///Description��Desc�B�Ӗ�:���� 
	D3D11_BUFFER_DESC desc{};
	//�ǂ��g����
	desc.Usage = D3D11_USAGE_DEFAULT;
	//�T�C�Y�ݒ�
	desc.ByteWidth = sizeof(Vertex) * 2;
	//���_�p�ɂ���
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//0��CPU����A�N�Z�X�ł��Ȃ��B
	//0�Ȃ珑���Ȃ��Ă�����
	desc.CPUAccessFlags = 0;
	//�쐬
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(&desc, nullptr, &m_vertexBuffer);
	//���_�Ɋւ��Ă͏I���

	/// <summary>
	/// �萔�o�b�t�@
	/// �r���[�s��ƃv���W�F�N�V�����𑗂�p
	/// </summary>
	//�T�C�Y
	desc.ByteWidth = sizeof(CMatrix);
	//�萔(CONSTANT)�悤�ɂ���
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	//�d�グ
	g_graphicsEngine->GetD3DDevice()->CreateBuffer(&desc, nullptr, &m_constantBuffer);
	//�萔���I���

	//�V�F�[�_�[�̐ݒ�
	m_Vshader.Load("Assets/shader/DebugMode.fx", "VSMain", Shader::EnType::VS);
	m_Pshader.Load("Assets/shader/DebugMode.fx", "PSMain", Shader::EnType::PS);

}

void DebugWireframe::Context()
{
	/// <summary>
	/// �f�o�C�X�R���e�L�X�g�ɂ��o��N���X�Ȃǂ̐ݒ肪
	/// �c���Ă��邽�ߏ㏑������(�X�V~)
	/// </summary>
}

//1�t���[������drawLine�͐��̐������s��
//�܂菈��������d��
void DebugWireframe::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
	//�܂��̓f�o�C�X�R���e�L�X�g���擾
	ID3D11DeviceContext* dc = g_graphicsEngine->GetD3DDeviceContext();

	/// <summary>
	/// ���_�V�F�[�_�[���f�o�C�X�R���e�L�X�g�ɐݒ�
	/// Shader�͐搶�̃��b�p�[�N���X
	/// ���ۂɎg��GetBody��void*�Ȃ̂�
	/// ID3D11VertexShader*�ɃL���X�g����K�v������
	/// </summary>
	dc->VSSetShader((ID3D11VertexShader*)m_Vshader.GetBody(), nullptr, 0);

	//	�s�N�Z���V�F�[�_�[���f�o�C�X�R���e�L�X�g�ɐݒ�
	//	���_�V�F�[�_�[�Ɠ��l
	dc->PSSetShader((ID3D11PixelShader*)m_Pshader.GetBody(), nullptr, 0);

	//�萔�o�b�t�@�̐ݒ�
	//mVP�̍X�V
	CMatrix VP;		//�r���[�s��ƃv���W�F�N�V�����s��
	//�r���[�ƃv���W�F�N�V�����̊|���Z
	auto v = g_camera3D.GetViewMatrix();
	auto p = g_camera3D.GetProjectionMatrix();
	VP.Mul(v, p);	//�r���[�s�� �~ �v���W�F�N�g�s��

	//�萔�o�b�t�@�ɓn�������ϐ����i�[(m_constantBuffer�̓��e���㏑��)
	dc->UpdateSubresource(m_constantBuffer, 0, nullptr, &VP, 0, 0);
	//�萔�o�b�t�@���f�o�C�X�R���e�L�X�g�ɐݒ�
	dc->VSSetConstantBuffers(
		0,//���W�X�^�̏ꏊ
		1,//�z��̗v�f��
		&m_constantBuffer//�萔�o�b�t�@�̃|�C���^
	);

	//�v�m�F�@
	//���_�o�b�t�@�̐ݒ�
	//�����̃X�g���C�h�ƃI�t�Z�b�g�p�ɕϐ������
	//�������|�C���^�̂���
	UINT ver = sizeof(Vertex);
	UINT offset = 0;
	//�萔�o�b�t�@���f�o�C�X�R���e�L�X�g�ɐݒ�
	dc->IASetVertexBuffers(
		0,					//���W�X�^�̏ꏊ
		1,					//�z��̗v�f��
		&m_vertexBuffer,	//�萔�o�b�t�@�̃|�C���^
		&ver,				//���_�̃T�C�Y,�X�g���C�h
		&offset				//���_�̔z��̃X�^�[�g�n�_�A�I�t�Z�b�g
	);

	//���_�̏�񂪂ǂ�Ȋ����ɓ����Ă���̂����킩��
	dc->IASetInputLayout(m_Vshader.GetInputLayout());

	//���_���ǂ�Ȋ����ɕ`�悷��̂�
	//����͒��_��̊Ԃɐ���`���ݒ�
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	//���_�����������ăh���[����֐�
	Vertex vers[2];

	vers[0].pos.x = from.x();
	vers[0].pos.y = from.y();
	vers[0].pos.z = from.z();

	vers[0].color.x = color.x();
	vers[0].color.y = color.y();
	vers[0].color.z = color.z();

	vers[1].pos.x = to.x();
	vers[1].pos.y = to.y();
	vers[1].pos.z = to.z();

	vers[1].color.x = color.x();
	vers[1].color.y = color.y();
	vers[1].color.z = color.z();

	/// <summary>
	/// ����ver��ver[0]�̃A�h���X
	/// Prepare�֐���ByteWidth�ŃT�C�Y��*2�ɂ��邱�Ƃ�
	/// ver�̗v�f������ł��邱�Ƃ��킩��B
	/// </summary>
	dc->UpdateSubresource(m_vertexBuffer, 0, nullptr, vers, 0, 0);

	//�`��
	//2�͒��_���A0�̓I�t�Z�b�g
	dc->Draw(2, 0);
}