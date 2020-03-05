/// <summary>
/// �X�v���C�g�p�̃V�F�[�_�[
/// </summary>

cbuffer cb : register(b0) {
	float4x4 wvp;		//���[���h�r���[�v���W�F�N�V�����s��
	float alpha;		//���l
};

struct VSInput {
	float4 pos : SV_Position;
	float2 uv  : TEXCOORD0;
};

struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

Texture2D<float4> colorTexture : register(t0);	//�J���[�e�N�X�`��
sampler Sampler : register(s0);

PSInput VSMain(VSInput In)
{
	PSInput psIn;
	psIn.pos = mul(wvp, In.pos);
	psIn.uv = In.uv;
	return psIn;
}
float4 PSMain(PSInput In) : SV_Target0
{
	//�e�N�X�`���J���[�����̂܂ܕԂ�
	float4 color = colorTexture.Sample(Sampler, In.uv);

	//���m�N���[��
	//float Y = 0.29900 * color.r + 0.58700 * color.g + 0.011400 * color.b;
	//color.r = Y;
	//color.g = Y;
	//color.b = Y;

	color.a*= alpha;
	return color;
}