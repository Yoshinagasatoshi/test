/// <summary>
/// �f�o�b�N���[�h�p�̃V�F�[�_�[
/// </summary>

//����������shadow���o�Ȃ��Ȃ�炵��

/// <summary>
/// �萔�o�b�t�@
/// </summary>
/// ���_�V�F�[�_�[�p�̒萔�o�b�t�@

cbuffer VSCb : register(b0) {
	float4x4 mVP;
}

/// <summary>
/// ���_�\����
/// </summary>
struct VSInput
{
	/// <summary>
	/// 
	/// </summary>
	float3 position : Position;		//���_���W
	float3 color	: Color;		//�J���[
};

//SV_Position�ŕԂ��悤�̍\����
struct PSInput
{
	//�̍\���̂�SV_Position���K�v
	//�ϐ����Ǝ����Ŗ��t�����Z�}���e�B�N�X��VSInput�Ƃ��Ԃ��Ă�OK
	float4 position : SV_Position;		//���_���W�B
	float3 color : Color;				//�J���[�B
};

//VSMain�̖߂�l��SV_Position���K�v
PSInput VSMain(VSInput input)
{
	PSInput output;
	output.position = mul(mVP, float4(input.position, 1.0f));
	output.color = input.color;
	return output;
}
//PSMain�̖߂�l��SV_Target���K�v
//�߂�l��color(float4)�ł���K�v������B
float4 PSMain(PSInput input) : SV_Target
{
	//output.position�͂��̊֐��ɗ���܂łɎg���I���

	//float4�ɂ���
	//1�̓A���t�@
	return float4(input.color,1);
}