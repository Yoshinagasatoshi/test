/*!
 * @brief	���f���V�F�[�_�[�B
 */


/////////////////////////////////////////////////////////////
// Shader Resource View
/////////////////////////////////////////////////////////////
//�A���x�h�e�N�X�`���B
Texture2D<float4> albedoTexture : register(t0);	
//�{�[���s��
StructuredBuffer<float4x4> boneMatrix : register(t1);

/////////////////////////////////////////////////////////////
// SamplerState
/////////////////////////////////////////////////////////////
sampler Sampler : register(s0);

/////////////////////////////////////////////////////////////
// �萔�o�b�t�@�B
/////////////////////////////////////////////////////////////
/*!
 * @brief	���_�V�F�[�_�[�ƃs�N�Z���V�F�[�_�[�p�̒萔�o�b�t�@�B
 */
cbuffer VSPSCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};
/// <summary>
/// ���C�g�̐�
/// </summary>
static const int Light_number = 4;
/*
�f�B���N�V�������C�g
*/
struct SDirectionLight {
	//�f�B���N�V�������C�g�̃x�N�g��
	float4 Direction[Light_number];
	//�f�B���N�V�������C�g�̐F
	float4 Color[Light_number];
};
/// <summary>
/// ���C�g�p�̒萔�o�b�t�@
/// </summary>
cbuffer LightCB : register(b0) {
	SDirectionLight directionLight; //�f�B���N�V�������C�g
	float3          eyePos;         //�J�����̎��_
	float           specPow;        //�X�y�L�������C�g�̍i��
	float3          Amblight;
}

/////////////////////////////////////////////////////////////
//�e��\����
/////////////////////////////////////////////////////////////
/*!
 * @brief	�X�L���Ȃ����f���̒��_�\���́B
 */
struct VSInputNmTxVcTangent
{
    float4 Position : SV_Position;			//���_���W�B
    float3 Normal   : NORMAL;				//�@���B
    float3 Tangent  : TANGENT;				//�ڃx�N�g���B
    float2 TexCoord : TEXCOORD0;			//UV���W�B
};
/*!
 * @brief	�X�L�����胂�f���̒��_�\���́B
 */
struct VSInputNmTxWeights
{
    float4 Position : SV_Position;			//���_���W�B
    float3 Normal   : NORMAL;				//�@���B
    float2 TexCoord	: TEXCOORD0;			//UV���W�B
    float3 Tangent	: TANGENT;				//�ڃx�N�g���B
    uint4  Indices  : BLENDINDICES0;		//���̒��_�Ɋ֘A�t������Ă���{�[���ԍ��Bx,y,z,w�̗v�f�ɓ����Ă���B4�{�[���X�L�j���O�B
    float4 Weights  : BLENDWEIGHT0;			//���̒��_�Ɋ֘A�t������Ă���{�[���ւ̃X�L���E�F�C�g�Bx,y,z,w�̗v�f�ɓ����Ă���B4�{�[���X�L�j���O�B
};

/*!
 * @brief	�s�N�Z���V�F�[�_�[�̓��́B
 */
struct PSInput{
	float4 Position 	: SV_POSITION;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float2 TexCoord 	: TEXCOORD0;
	float3 worldPos		: TEXCOORD1;	//���[���h���W�B����U
};
/*!
 *@brief	�X�L���s����v�Z�B
 */
float4x4 CalcSkinMatrix(VSInputNmTxWeights In)
{
	float4x4 skinning = 0;	
	float w = 0.0f;
	[unroll]
    for (int i = 0; i < 3; i++)
    {
        skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
        w += In.Weights[i];
    }
    
    skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
    return skinning;
}
/*!--------------------------------------------------------------------------------------
 * @brief	�X�L���Ȃ����f���p�̒��_�V�F�[�_�[�B
-------------------------------------------------------------------------------------- */
PSInput VSMain( VSInputNmTxVcTangent In ) 
{
	PSInput psInput = (PSInput)0;
	float4 pos = mul(mWorld, In.Position);

	//���[���h���W���s�N�Z���V�F�[�_�ɓn���B
	psInput.worldPos = pos;

	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.TexCoord = In.TexCoord;
	psInput.Normal = normalize(mul(mWorld, In.Normal));
	psInput.Tangent = normalize(mul(mWorld, In.Tangent));
	return psInput;
}

/*!--------------------------------------------------------------------------------------
 * @brief	�X�L�����胂�f���p�̒��_�V�F�[�_�[�B
 * �S�Ă̒��_�ɑ΂��Ă��̃V�F�[�_�[���Ă΂��B
 * In��1�̒��_�f�[�^�BVSInputNmTxWeights�����Ă݂悤�B
-------------------------------------------------------------------------------------- */
PSInput VSMainSkin( VSInputNmTxWeights In ) 
{
	PSInput psInput = (PSInput)0;
	
	///////////////////////////////////////////////////
	//��������X�L�j���O���s���Ă���ӏ��B
	//�X�L���s����v�Z�B
	///////////////////////////////////////////////////
	float4x4 skinning = 0;	
	float4 pos = 0;
	{
	
		float w = 0.0f;
	    for (int i = 0; i < 3; i++)
	    {
			//boneMatrix�Ƀ{�[���s�񂪐ݒ肳��Ă��āA
			//In.indices�͒��_�ɖ��ߍ��܂ꂽ�A�֘A���Ă���{�[���̔ԍ��B
			//In.weights�͒��_�ɖ��ߍ��܂ꂽ�A�֘A���Ă���{�[���̃E�F�C�g�B
	        skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
	        w += In.Weights[i];
	    }
	    //�Ō�̃{�[�����v�Z����B
	    skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
	  	//���_���W�ɃX�L���s�����Z���āA���_�����[���h��Ԃɕϊ��B
		//mul�͏�Z���߁B
	    pos = mul(skinning, In.Position);
	}
	
	psInput.Normal = normalize( mul(skinning, In.Normal) );
	psInput.Tangent = normalize( mul(skinning, In.Tangent) );
	
	//���[���h���W���s�N�Z���V�F�[�_�ɓn���B
	psInput.worldPos = pos;

	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.TexCoord = In.TexCoord;
    return psInput;
}
//--------------------------------------------------------------------------------------
// �s�N�Z���V�F�[�_�[�̃G���g���֐��B
//--------------------------------------------------------------------------------------
float4 PSMain(PSInput In) : SV_Target0
{
	/// <summary>
	/// ���C�g�̃R�[�h
	/// </summary>
	//albedo�e�N�X�`������J���[���t�F�b�`����B
	//�g�U����
	float4 albedoColor = albedoTexture.Sample(Sampler, In.TexCoord);
	float3 lig;
    for (int i = 0; i < Light_number; i++) {
	lig += max(0.0f, dot(In.Normal * -1.0f, directionLight.Direction[i].rgb)) * directionLight.Color[i];
	}
	//for (int i = 0; i < Light_number; i++)
	////�f�B���N�V�������C�g�̋��ʔ��ˌ����v�Z����B
	{
		//���K�@���ʔ��˂��v�Z���Ȃ����B
		//�P�@���˃x�N�g��R�����߂�
		float3 R = directionLight.Direction[0].rgb
			+ 2 * dot(In.Normal, -directionLight.Direction[0].rgb)
			* In.Normal;
		//-toEyeDir + 2 * dot(In.Normal, -directionLight.direction) * In.NORMAL;
	//�Q�@���_���烉�C�g�𓖂Ă镨�̂ɐL�т�x�N�g��E�����߂�B
		float3 E = normalize(In.worldPos - eyePos);
		//�P�ƂQ�ŋ��܂����x�N�g���̓��ς��v�Z����B
		float specPower = max(0, dot(R, -E));
		//3 �X�y�L�������˂����C�g�ɒǉ�����B
		lig += directionLight.Color[0].xyz * pow(specPower, specPow);
	
		//4�@�����𓖂Ă�B
		lig += Amblight;
	}
	float4 finalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	finalColor.xyz = albedoColor.xyz * lig;

	return finalColor;
	//return albedoTexture.Sample(Sampler, In.TexCoord);
}
//-----------------------------------------------
// �V���G�b�g�`��p�̃s�N�Z���V�F�[�_�[�̃G���g���[�֐�
//-----------------------------------------------
