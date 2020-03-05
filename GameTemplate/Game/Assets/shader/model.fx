/*!
 * @brief	モデルシェーダー。
 */


/////////////////////////////////////////////////////////////
// Shader Resource View
/////////////////////////////////////////////////////////////
//アルベドテクスチャ。
Texture2D<float4> albedoTexture : register(t0);	
//ボーン行列
StructuredBuffer<float4x4> boneMatrix : register(t1);

/////////////////////////////////////////////////////////////
// SamplerState
/////////////////////////////////////////////////////////////
sampler Sampler : register(s0);

/////////////////////////////////////////////////////////////
// 定数バッファ。
/////////////////////////////////////////////////////////////
/*!
 * @brief	頂点シェーダーとピクセルシェーダー用の定数バッファ。
 */
cbuffer VSPSCb : register(b0){
	float4x4 mWorld;
	float4x4 mView;
	float4x4 mProj;
};
/// <summary>
/// ライトの数
/// </summary>
static const int Light_number = 4;
/*
ディレクションライト
*/
struct SDirectionLight {
	//ディレクションライトのベクトル
	float4 Direction[Light_number];
	//ディレクションライトの色
	float4 Color[Light_number];
};
/// <summary>
/// ライト用の定数バッファ
/// </summary>
cbuffer LightCB : register(b0) {
	SDirectionLight directionLight; //ディレクションライト
	float3          eyePos;         //カメラの視点
	float           specPow;        //スペキュラライトの絞り
	float3          Amblight;
}

/////////////////////////////////////////////////////////////
//各種構造体
/////////////////////////////////////////////////////////////
/*!
 * @brief	スキンなしモデルの頂点構造体。
 */
struct VSInputNmTxVcTangent
{
    float4 Position : SV_Position;			//頂点座標。
    float3 Normal   : NORMAL;				//法線。
    float3 Tangent  : TANGENT;				//接ベクトル。
    float2 TexCoord : TEXCOORD0;			//UV座標。
};
/*!
 * @brief	スキンありモデルの頂点構造体。
 */
struct VSInputNmTxWeights
{
    float4 Position : SV_Position;			//頂点座標。
    float3 Normal   : NORMAL;				//法線。
    float2 TexCoord	: TEXCOORD0;			//UV座標。
    float3 Tangent	: TANGENT;				//接ベクトル。
    uint4  Indices  : BLENDINDICES0;		//この頂点に関連付けされているボーン番号。x,y,z,wの要素に入っている。4ボーンスキニング。
    float4 Weights  : BLENDWEIGHT0;			//この頂点に関連付けされているボーンへのスキンウェイト。x,y,z,wの要素に入っている。4ボーンスキニング。
};

/*!
 * @brief	ピクセルシェーダーの入力。
 */
struct PSInput{
	float4 Position 	: SV_POSITION;
	float3 Normal		: NORMAL;
	float3 Tangent		: TANGENT;
	float2 TexCoord 	: TEXCOORD0;
	float3 worldPos		: TEXCOORD1;	//ワールド座標。解説６
};
/*!
 *@brief	スキン行列を計算。
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
 * @brief	スキンなしモデル用の頂点シェーダー。
-------------------------------------------------------------------------------------- */
PSInput VSMain( VSInputNmTxVcTangent In ) 
{
	PSInput psInput = (PSInput)0;
	float4 pos = mul(mWorld, In.Position);

	//ワールド座標をピクセルシェーダに渡す。
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
 * @brief	スキンありモデル用の頂点シェーダー。
 * 全ての頂点に対してこのシェーダーが呼ばれる。
 * Inは1つの頂点データ。VSInputNmTxWeightsを見てみよう。
-------------------------------------------------------------------------------------- */
PSInput VSMainSkin( VSInputNmTxWeights In ) 
{
	PSInput psInput = (PSInput)0;
	
	///////////////////////////////////////////////////
	//ここからスキニングを行っている箇所。
	//スキン行列を計算。
	///////////////////////////////////////////////////
	float4x4 skinning = 0;	
	float4 pos = 0;
	{
	
		float w = 0.0f;
	    for (int i = 0; i < 3; i++)
	    {
			//boneMatrixにボーン行列が設定されていて、
			//In.indicesは頂点に埋め込まれた、関連しているボーンの番号。
			//In.weightsは頂点に埋め込まれた、関連しているボーンのウェイト。
	        skinning += boneMatrix[In.Indices[i]] * In.Weights[i];
	        w += In.Weights[i];
	    }
	    //最後のボーンを計算する。
	    skinning += boneMatrix[In.Indices[3]] * (1.0f - w);
	  	//頂点座標にスキン行列を乗算して、頂点をワールド空間に変換。
		//mulは乗算命令。
	    pos = mul(skinning, In.Position);
	}
	
	psInput.Normal = normalize( mul(skinning, In.Normal) );
	psInput.Tangent = normalize( mul(skinning, In.Tangent) );
	
	//ワールド座標をピクセルシェーダに渡す。
	psInput.worldPos = pos;

	pos = mul(mView, pos);
	pos = mul(mProj, pos);
	psInput.Position = pos;
	psInput.TexCoord = In.TexCoord;
    return psInput;
}
//--------------------------------------------------------------------------------------
// ピクセルシェーダーのエントリ関数。
//--------------------------------------------------------------------------------------
float4 PSMain(PSInput In) : SV_Target0
{
	/// <summary>
	/// ライトのコード
	/// </summary>
	//albedoテクスチャからカラーをフェッチする。
	//拡散反射
	float4 albedoColor = albedoTexture.Sample(Sampler, In.TexCoord);
	float3 lig;
    for (int i = 0; i < Light_number; i++) {
	lig += max(0.0f, dot(In.Normal * -1.0f, directionLight.Direction[i].rgb)) * directionLight.Color[i];
	}
	//for (int i = 0; i < Light_number; i++)
	////ディレクションライトの鏡面反射光を計算する。
	{
		//実習　鏡面反射を計算しなさい。
		//１　反射ベクトルRを求める
		float3 R = directionLight.Direction[0].rgb
			+ 2 * dot(In.Normal, -directionLight.Direction[0].rgb)
			* In.Normal;
		//-toEyeDir + 2 * dot(In.Normal, -directionLight.direction) * In.NORMAL;
	//２　視点からライトを当てる物体に伸びるベクトルEを求める。
		float3 E = normalize(In.worldPos - eyePos);
		//１と２で求まったベクトルの内積を計算する。
		float specPower = max(0, dot(R, -E));
		//3 スペキュラ反射をライトに追加する。
		lig += directionLight.Color[0].xyz * pow(specPower, specPow);
	
		//4　環境光を当てる。
		lig += Amblight;
	}
	float4 finalColor = float4(0.0f, 0.0f, 0.0f, 1.0f);
	finalColor.xyz = albedoColor.xyz * lig;

	return finalColor;
	//return albedoTexture.Sample(Sampler, In.TexCoord);
}
//-----------------------------------------------
// シルエット描画用のピクセルシェーダーのエントリー関数
//-----------------------------------------------
