/// <summary>
/// デバックモード用のシェーダー
/// </summary>

//これをするとshadowが出なくなるらしい

/// <summary>
/// 定数バッファ
/// </summary>
/// 頂点シェーダー用の定数バッファ

cbuffer VSCb : register(b0) {
	float4x4 mVP;
}

/// <summary>
/// 頂点構造体
/// </summary>
struct VSInput
{
	/// <summary>
	/// 
	/// </summary>
	float3 position : Position;		//頂点座標
	float3 color	: Color;		//カラー
};

//SV_Positionで返すようの構造体
struct PSInput
{
	//個の構造体はSV_Positionが必要
	//変数名と自分で名付けたセマンティクスはVSInputとかぶってもOK
	float4 position : SV_Position;		//頂点座標。
	float3 color : Color;				//カラー。
};

//VSMainの戻り値はSV_Positionが必要
PSInput VSMain(VSInput input)
{
	PSInput output;
	output.position = mul(mVP, float4(input.position, 1.0f));
	output.color = input.color;
	return output;
}
//PSMainの戻り値はSV_Targetが必要
//戻り値はcolor(float4)である必要がある。
float4 PSMain(PSInput input) : SV_Target
{
	//output.positionはこの関数に来るまでに使い終わる

	//float4にする
	//1はアルファ
	return float4(input.color,1);
}