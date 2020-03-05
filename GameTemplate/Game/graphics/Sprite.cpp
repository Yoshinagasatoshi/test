#include "stdafx.h"
#include "Sprite.h"

//���_�\���́B
struct Vertex {
	CVector4	pos;		//���W�B
	CVector2	uv;			//UV���W�B
};

Sprite::Sprite()
{
}


Sprite::~Sprite()
{
	if (m_vertexBuffer != nullptr) {
		m_vertexBuffer->Release();
	}
	if (m_indexBuffer != nullptr) {
		m_indexBuffer->Release();
	}
	if (m_cb != nullptr) {
		m_cb->Release();
	}
	if (m_texture != nullptr) {
		m_texture->Release();
	}
	if (m_samplerState != nullptr) {
		m_samplerState->Release();
	}
}

void Sprite::Init(const wchar_t* textureFilePath, float w, float h)
{
	//���ʂ̏������������Ă�
	InitCommon(w,h);

	//�e�N�X�`�������[�h�B
	LoadTexture(textureFilePath);
}

void Sprite::Init(ID3D11ShaderResourceView* srv, float w, float h)
{
	//���ʂ̏������������Ă�
	InitCommon(w,h);

	m_texture = srv;
	m_texture->AddRef();
}

void Sprite::InitCommon(float w, float h)
{
	m_size.x = w;
	m_size.y = h;
	//���_�o�b�t�@�̏�����
	InitVertexBuffer(w, h);
	//�C���f�b�N�X�o�b�t�@�̏�����
	InitIndexBuffer();
	//�T���v���X�e�[�g�̏������B
	InitSamplerState();
	//�V�F�[�_�[�̃��[�h
	LoadShader();
	//�萔�o�b�t�@��������
	InitConstantBuffer();
}

void Sprite::LoadShader()
{
	//�V�F�[�_�[�����[�h����B
	m_ps.Load("Assets/shader/sprite.fx", "PSMain", Shader::EnType::PS);
	m_vs.Load("Assets/shader/sprite.fx", "VSMain", Shader::EnType::VS);
}
void Sprite::InitConstantBuffer()
{
	//�ǂ�ȃo�b�t�@���쐬����̂�������bufferDesc�ɐݒ肷��B
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;						//�o�b�t�@�őz�肳��Ă���A�ǂݍ��݂���я������ݕ��@�B
	desc.ByteWidth = (((sizeof(ConstantBuffer) - 1) / 16) + 1) * 16;	//�萔�o�b�t�@��16�o�C�g�A���C�����g�ɂȂ��Ă���K�v������B
															//�A���C�����g���ā��o�b�t�@�̃T�C�Y��16�̔{���Ƃ������Ƃł��B
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//�o�b�t�@���ǂ̂悤�ȃp�C�v���C���Ƀo�C���h���邩���w�肷��B
															//�萔�o�b�t�@�Ƀo�C���h����̂ŁAD3D11_BIND_CONSTANT_BUFFER���w�肷��B
	desc.CPUAccessFlags = 0;								//CPU �A�N�Z�X�̃t���O�ł��B
															//CPU�A�N�Z�X���s�v�ȏꍇ��0�B
	//�쐬�B
	auto d3dDevice = g_graphicsEngine->GetD3DDevice();
	d3dDevice->CreateBuffer(&desc, NULL, &m_cb);
}
void Sprite::InitVertexBuffer(float w, float h)
{
	//��Œ�`�������_���g�p���Ē��_�o�b�t�@���쐬
	//���_�o�b�t�@���쐬���邽�߂ɂ�D3D11_BUFFER_DESC��D3D11_SUBEWSORCE_DATA��ݒ肷��K�v������B
	float halfW = w * 0.5f;
	float halfH = h * 0.5f;

	//���_�o�b�t�@�̌��f�[�^�B
	//�����Ŏw�肳�ꂽ�c�A���̃T�C�Y�̔|���B
	Vertex vertices[4] = {
		{
			CVector4(-halfW, -halfH, 0.0f, 1.0f),
			CVector2(0.0f, 1.0f),
		},
		{
			CVector4(halfW, -halfH, 0.0f, 1.0f),
			CVector2(1.0f, 1.0f),
		},
		{
			CVector4(-halfW, halfH, 0.0f, 1.0f),
			CVector2(0.0f, 0.0f)
		},
		{
			CVector4(halfW, halfH, 0.0f, 1.0f),
			CVector2(1.0f, 0.0f)
		}
	};

	//���_�o�b�t�@���쐬���邽�߂̏���ݒ肷��B
	//���_�o�b�t�@���쐬���邽�߂ɂ�D3D11_BUFFER_DESC��D3D11_SUBRESOUECE_DATA��ݒ肷��B
	D3D11_BUFFER_DESC desc;							
	ZeroMemory(&desc, sizeof(desc));				//�\���̂̃����o��0�ŏ���������B
	desc.Usage = D3D11_USAGE_DEFAULT;				//�o�b�t�@�[�őz�肳��Ă���ǂݍ��݁A����я������݂̕��@

	desc.StructureByteStride = sizeof(Vertex);		//�\���̃o�b�t�@�̂P�v�f�̃T�C�Y�B(���_����̃T�C�Y�ł��B)
	desc.ByteWidth = sizeof(Vertex) * 4;			//���_�o�b�t�@�̃T�C�Y�B
													//�l�p�`�̃|���S����`�悷��̂ŁA�S���_�K�v�B
													//�Ȃ̂ŁA��̒��_�̃T�C�Y�~�S�̃T�C�Y���K�v�ƂȂ�B
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;		//�쐬����o�b�t�@�����_�o�b�t�@�ł��邱�Ƃ��w�肷��B
	desc.CPUAccessFlags = 0;						//CPU���璸�_�o�b�t�@�ɃA�N�Z�X���邩�̃t���O�B
													//�ҏW������͂��Ȃ��̂ŁA�O�ɂ��Ă����B

	//�����āA���f�[�^�̏������B
	//���f�[�^���Ȃ��Ȃ炱��͂���Ȃ��B
	//����͕K�v�B
	D3D11_SUBRESOURCE_DATA initData;				
	ZeroMemory(&initData, sizeof(initData));		//�\���̂̃����o��0�ŏ���������B
	initData.pSysMem = vertices;					//���f�[�^�̃A�h���X�����B

	//�����܂Őݒ肵�Ă��������g���āAVRAM��ɒ��_�o�b�t�@���쐬����B
	auto d3dDevice = g_graphicsEngine->GetD3DDevice();
	d3dDevice->CreateBuffer(&desc, &initData, &m_vertexBuffer);
}

void Sprite::InitIndexBuffer()
{
	//���ꂪ�C���f�b�N�X�B���_�ԍ�
	int index[6] = {
		0,1,2,		//�O�p�`���
		2,1,3		//�O�p�`���
	};

	//��Œ�`�����C���f�b�N�X���g�p���ăC���f�b�N�X�o�b�t�@���쐬����
	//�C���f�b�N�X�o�b�t�@�����_�o�b�t�@�Ɠ�����
	//D3D11_BUFFER_DESC��D3D11_SUBRESOURCE_DATA������������K�v������
	D3D11_BUFFER_DESC desc;							//�\���̂̃����o��0�ŏ���������B
	ZeroMemory(&desc, sizeof(desc));
	desc.Usage = D3D11_USAGE_DEFAULT;
	//desc.StructureByteStride = 4;					//�C���f�b�N�X����̃T�C�Y�Bshort�Ȃ̂�2�o�C�g�B
	desc.ByteWidth = sizeof(index);				//�C���f�b�N�X�o�b�t�@�̃T�C�Y�B
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;		//�쐬����o�b�t�@���C���f�b�N�X�o�b�t�@�ł��邱�Ƃ��w�肷��B
	desc.CPUAccessFlags = 0;						//CPU���璸�_�o�b�t�@�ɃA�N�Z�X���邩�̃t���O�B
													//�ҏW������͂��Ȃ��̂ŁA�O�ɂ��Ă����B

	D3D11_SUBRESOURCE_DATA initData = { 0 };			//�\���̂̃����o��0�ŏ���������B
	initData.pSysMem = index;						//���f�[�^�̃A�h���X��������B

	//�����܂Őݒ肵�Ă��������g���āAVRAM��ɃC���f�b�N�X�o�b�t�@���쐬����B
	auto d3dDevice = g_graphicsEngine->GetD3DDevice();
	d3dDevice->CreateBuffer(&desc, &initData, &m_indexBuffer);
}

void Sprite::InitSamplerState()
{
	//�e�N�X�`���̃T���v�����O���@���w�肷�邽�߂̃T���v���X�e�[�g���쐬�B
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	//Clamp+�o�C���j�A�t�B���^��������T���v���X�e�[�g���쐬����B
	desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	g_graphicsEngine->GetD3DDevice()->CreateSamplerState(&desc, &m_samplerState);
}
void Sprite::LoadTexture(const wchar_t* textureFIlePath)
{
	//D3D�f�o�C�X���擾�B
	auto d3dDevice = g_graphicsEngine->GetD3DDevice();

	DirectX::CreateDDSTextureFromFileEx(
		d3dDevice,
		textureFIlePath,
		0,
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_SHADER_RESOURCE,
		0,
		0,
		false,
		nullptr,
		&m_texture);
}

void Sprite::Update(const CVector3& trans, CQuaternion rot, CVector3 scale, CVector2 pivot)
{
	//���[���h�s����v�Z����
	CMatrix mTrans, mRot, mScale;
	mTrans.MakeTranslation(trans);
	mRot.MakeRotationFromQuaternion(rot);
	mScale.MakeScaling(scale);
	m_world.Mul(mScale, mRot);
	m_world.Mul(m_world, mTrans);
}
void Sprite::Draw()
{
	//�̏����Ă����R�[�h�A�R�����g�A�E�g���ĕ���
	/*
	////�萔�o�b�t�@���X�V�B
	//SSpriteCB cb;
	////���[���h�~�r���[�~�v���W�F�N�V�����s����v�Z�B
	//cb.mWVP.Mul(m_world, mView);
	//cb.mWVP.Mul(cb.mWVP, mProj);
	////�萔�o�b�t�@�̓��e�����C������������VRAM�ɃR�s�[�B
	//d3dDeviceContext->UpdateSubresource(m_cb, 0, nullptr, &cb, 0, 0);
	////�萔�o�b�t�@�����W�X�^b0�Ƀo�C���h����B
	//d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_cb);
	////�e�N�X�`�������W�X�^t0�Ƀo�C���h����B
	//d3dDeviceContext->PSSetShaderResources(0, 1, &m_texture);
	////�T���v���X�e�[�g�����W�X�^s0�Ƀo�C���h����B
	//d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState);

	////���_�o�b�t�@��ݒ�B
	//UINT stride = sizeof(Vertex);
	//UINT offset = 0;

	*/

	//�f�o�C�X�R���e�L�X�g�����������Ă���B
	auto d3dDeviceContext = g_graphicsEngine->GetD3DDeviceContext();

	unsigned int vertexSize = sizeof(Vertex);	//���_�T�C�Y
	unsigned int offset = 0;

	d3dDeviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &vertexSize, &offset);
	//�C���f�b�N�X�o�b�t�@��ݒ�B
	d3dDeviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	//���_�V�F�[�_�[��ݒ�B
	d3dDeviceContext->VSSetShader((ID3D11VertexShader*)m_vs.GetBody(), nullptr, 0);
	//�s�N�Z���V�F�[�_��ݒ�B
	d3dDeviceContext->PSSetShader((ID3D11PixelShader*)m_ps.GetBody(), nullptr, 0);
	//���_���C�A�E�g��ݒ�B
	d3dDeviceContext->IASetInputLayout(m_vs.GetInputLayout());
	//�e�N�X�`����ݒ�
	d3dDeviceContext->PSSetShaderResources(0, 1, &m_texture);
	//�T���v���X�e�[�g��ݒ�
	d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState);

	//���[���h�r���[�v���W�F�N�V�����s����쐬
	ConstantBuffer cb;
	cb.WVP = m_world;
	//2D�J�����N���X���Ȃ����I����
	cb.WVP.Mul(cb.WVP,g_camera2D.GetViewMatrix());
	cb.WVP.Mul(cb.WVP,g_camera2D.GetProjectionMatrix());
	cb.alpha = m_alpha;

	d3dDeviceContext->UpdateSubresource(m_cb, 0, NULL, &cb, 0, 0);
	d3dDeviceContext->VSSetConstantBuffers(0, 1, &m_cb);
	d3dDeviceContext->PSSetConstantBuffers(0, 1, &m_cb);
	//�v���~�e�B�u�̃g�|���W�[��ݒ�B
	d3dDeviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//�����܂Őݒ肵�����e�Ńh���[
	d3dDeviceContext->DrawIndexed(6, 0, 0);
	
}