#pragma once
#include "bulletPhysics/src/LinearMath/btIDebugDraw.h"
#include "graphics/Shader.h"

class DebugWireframe :
	public btIDebugDraw
{
public:
	DebugWireframe();
	~DebugWireframe();
	/// <summary>
	/// 設定の準備
	/// </summary>
	void Prepare();
	/// <summary>
	/// デバイスコンテキストに設定
	/// 1f内に一回
	/// </summary>
	void Context();
	//必須
	/// <summary>
	/// 線ごとに一回ずつ呼ばれる
	/// </summary>
	/// <param name="from"></param>
	/// <param name="to"></param>
	/// <param name="color"></param>
	void	drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
	void	setDebugMode(int debugMode) override {};
	int		getDebugMode() const override
	{
		return true;
	}

	//何もしなくてもいい。必要であれば定義
	void	drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distans, int lifeTime, const btVector3& color) override {};
	void	reportErrorWarning(const char* warningString) override {};
	void	draw3dText(const btVector3& location, const char* textString) override {};
private:
	struct Vertex
	{
		CVector3 pos;
		CVector3 color;
	};

	ID3D11Buffer*		m_vertexBuffer = nullptr;		//頂点バッファ
	ID3D11Buffer*		m_constantBuffer = nullptr;		//定数バッファ
	Shader m_Vshader;	//頂点シェーダ
	Shader m_Pshader;	//ピクセルシェーダ
};