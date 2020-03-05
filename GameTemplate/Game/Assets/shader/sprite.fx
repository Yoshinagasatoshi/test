/// <summary>
/// スプライト用のシェーダー
/// </summary>

cbuffer cb : register(b0) {
	float4x4 wvp;		//ワールドビュープロジェクション行列
	float alpha;		//α値
};

struct VSInput {
	float4 pos : SV_Position;
	float2 uv  : TEXCOORD0;
};

struct PSInput {
	float4 pos : SV_POSITION;
	float2 uv  : TEXCOORD0;
};

Texture2D<float4> colorTexture : register(t0);	//カラーテクスチャ
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
	//テクスチャカラーをそのまま返す
	float4 color = colorTexture.Sample(Sampler, In.uv);

	//モノクローム
	//float Y = 0.29900 * color.r + 0.58700 * color.g + 0.011400 * color.b;
	//color.r = Y;
	//color.g = Y;
	//color.b = Y;

	color.a*= alpha;
	return color;
}