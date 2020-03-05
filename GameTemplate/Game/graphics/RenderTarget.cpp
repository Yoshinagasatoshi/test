#include "stdafx.h"
#include "RenderTarget.h"

RenderTarget::RenderTarget()
{
	if (m_depthStencilView != nullptr) {
		m_depthStencilView->Release();
		m_depthStencilView = nullptr;
	}
	if (m_depthStencilTexture != nullptr) {
		m_depthStencilTexture->Release();
		m_depthStencilTexture = nullptr;
	}
	if (m_renderTargetView != nullptr) {
		m_renderTargetView->Release();
		m_renderTargetView = nullptr;
	}
	if (m_renderTargetTexture != nullptr) {
		m_renderTargetTexture->Release();
		m_renderTargetTexture = nullptr;
	}
	if (m_RT_SRV != nullptr) {
		m_RT_SRV->Release();
	}

}

void RenderTarget::Create(unsigned int w, unsigned int h, DXGI_FORMAT textFormat)
{
	//D3Dデバイスを取得
	auto d3dDevice = g_graphicsEngine->GetD3DDevice();
	//1.レンダリングターゲットとなるテクスチャを作成。
	D3D11_TEXTURE2D_DESC texDesc = { 0 };
	{
		/// <summary>
		/// テクスチャを作成していく
		/// </summary>
		//テクスチャの横の解像度。
		texDesc.Width = w;
		//縦
		texDesc.Height = h;
		//ミップマップレベル。数値が大きいほど遠くの画面がぼやけるようになる。
		//1でいい。
		texDesc.MipLevels = 1;
		//テクスチャ配列のサイズ。1でいい。
		texDesc.ArraySize = 1;
		/// <summary>
		/// テクスチャのフォーマット
		/// RGBAの各要素に何bit振り分けて合計値分の
		/// フォーマットにする。
		/// 今回は16bit*4の64bitフォーマットにする
		/// </summary>
		texDesc.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
		//テクスチャの使用用途を決める。
		texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
		//マルチサンプリング(MSAA)の数。周囲4pixの
		//深度の大きさによってブレンド率を高くする。
		//1でいい。
		texDesc.SampleDesc.Count = 1;
		//マルチサンプリング(MSAA)の品質。0でいい
		texDesc.SampleDesc.Quality = 0;
		//テクスチャの読み書きのされ方
		texDesc.Usage = D3D11_USAGE_DEFAULT;
		//CPUからのアクセスの仕方
		texDesc.CPUAccessFlags = 0;
		//0でよい
		texDesc.MiscFlags = 0;
		//テクスチャを作成。
		d3dDevice->CreateTexture2D(&texDesc, nullptr, &m_renderTargetTexture);
	}
	//2.レンダリングターゲットビューの作成
	{
		//レンダリングターゲットビューを作成するためのデータを設定する。。
		D3D11_RENDER_TARGET_VIEW_DESC viewDesc;
		//フォーマットはテクスチャと同じにする。
		viewDesc.Format = texDesc.Format;
		//2Dテクスチャを扱うことを指定する。
		viewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		//0でいい。
		viewDesc.Texture2D.MipSlice = 0;
		//レンダリングターゲットビューの作成。
		d3dDevice->CreateRenderTargetView(m_renderTargetTexture, &viewDesc, &m_renderTargetView);
	}
	//3.シェーダーリソースビューの作成
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		//ふぉーまっとはテクスチャと同じでよい
		srvDesc.Format = texDesc.Format;
		//二次元テクスチャとして扱う。
		srvDesc.ViewDimension = D3D_SRV_DIMENSION_TEXTURE2D;
		//ミップマップレベル・テクスチャと同じでよい。
		srvDesc.Texture2D.MipLevels = texDesc.MipLevels;
		//0でいい。
		srvDesc.Texture2D.MostDetailedMip = 0;
		//SRV作成
		d3dDevice->CreateShaderResourceView(m_renderTargetTexture, &srvDesc, &m_RT_SRV);
	}
	//4.デプスステンシルビューの作成
	D3D11_TEXTURE2D_DESC depthTexDesc = texDesc;
	{
		//デプスステンシルビューにバインド(関連付け)する。
		depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		//32bit浮動小数点のデプスステンシルバッファを作成。
		depthTexDesc.Format = DXGI_FORMAT_D32_FLOAT;
		//デプスステンシルテクスチャを作成する。
		d3dDevice->CreateTexture2D(&depthTexDesc, nullptr, &m_depthStencilTexture);
	}
	//5.デプスステンシルビューの作成
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
		//フォーマットはデプスステンシルと同じにする
		depthStencilViewDesc.Format = depthTexDesc.Format;
		//2Dテクスチャを扱うことを指定する。
		depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		//ミップマップは0でﾖｼ!
		depthStencilViewDesc.Texture2D.MipSlice = 0;
		//こいつも0
		depthStencilViewDesc.Flags = 0;
		//デプスステンシルビューを作成。
		d3dDevice->CreateDepthStencilView(m_depthStencilTexture, &depthStencilViewDesc, &m_depthStencilView);
	}
}


void RenderTarget::ClearRenderTarget(float* clearColor)
{
	auto d3dDeviceContect = g_graphicsEngine->GetD3DDeviceContext();
	//レンダーターゲットをクリア
	d3dDeviceContect->ClearRenderTargetView(m_renderTargetView, clearColor);
	d3dDeviceContect->ClearDepthStencilView(m_depthStencilView, D3D11_CLEAR_DEPTH,1.0f,0);
}