#pragma once
/// <summary>
/// レンダリングターゲットクラス
/// </summary>
class RenderTarget 
{
public:
	RenderTarget();

	/// <summary>
	/// レンダリングターゲットの作成。
	/// 描画の書き込み先やぞ
	/// </summary>
	/// <param name="w">テクスチャの幅</param>
	/// <param name="h">テクスチャの高さ</param>
	/// <param name="textFormat">テクスチャのフォーマット</param>
	void Create(unsigned int w, unsigned int h, DXGI_FORMAT textFormat);

	/// <summary>
	/// レンダリングターゲットとデプスステンシルビューのクリア
	/// </summary>
	/// <param name="clearColor"></param>
	void ClearRenderTarget(float* clearColor);

	/// <summary>
	/// レンダリングターゲットビューを取得。
	/// </summary>
	ID3D11RenderTargetView* GetRenderTargetView()
	{
		return m_renderTargetView;
	}

	/// <summary>
	/// デプスステンシルビューを取得
	/// </summary>
	/// <returns></returns>
	ID3D11DepthStencilView* GetDepthStensilView()
	{
		return m_depthStencilView;
	}

	/// <summary>
	/// シェーダーリソースビューを取得
	/// </summary>
	/// <returns>レンダリングターゲットのSRV</returns>
	ID3D11ShaderResourceView* GetRenderTargetSRV()
	{
		return m_RT_SRV;
	}
private:
	//レンダリングターゲット関係のメンバ変数
	ID3D11Texture2D* m_renderTargetTexture = nullptr;			//!レンダリングターゲットに書き込まれるテクスチャ
	ID3D11RenderTargetView* m_renderTargetView = nullptr;		//!レンダリングターゲットビュー。書き込み先を決めるやつ。
	ID3D11ShaderResourceView* m_RT_SRV = nullptr;				//!シェーダーリソースビュー。色を書き込むやつ。
	ID3D11Texture2D* m_depthStencilTexture = nullptr;			//!デプスステンシルビューに使われるtexture
	ID3D11DepthStencilView* m_depthStencilView = nullptr;		//!デプスステンシルビュー。深度型抜き。
};