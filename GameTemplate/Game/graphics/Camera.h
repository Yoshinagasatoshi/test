#pragma once
class Camera
{
public:
	/// <summary>
	/// 射影行列の更新方法
	/// </summary>
	enum EnUpdateProjMatrixFunc {
		enUpdateProjMatrixFunc_Perspective,		//!<透視射影行列。遠近法を活かしたいならこちら
		enUpdateProjMatrixFunc_Ortho			//!<平行投影。2D的な表現をしたいときはこちら
	};
	/*!
	 * @brief	カメラ行列、プロジェクション行列の更新。
	 *@details
	 * 現在設定されている、注視点、視点、画角などの情報をもとに
	 * カメラ行列とプロジェクション行列を更新します。
	 * この関数を呼ばないと、カメラは動かないので注意が必要です。
	 */
	void Update();
	/*!
	 * @brief	カメラ行列を取得。
	 */
	CMatrix GetViewMatrix()
	{
		return m_viewMatrix;
	}
	/*!
	 * @brief	プロジェクション行列を取得。
	 */
	CMatrix GetProjectionMatrix() 
	{
		return m_projMatrix;
	}
	/*!
	 * @brief	視点を取得。
	 */
	CVector3 GetPosition()
	{
		return m_position;
	}
	/*!
	 * @brief	注視点を取得。
	 */
	CVector3 GetTarget()
	{
		return m_target;
	}
	/*!
	 * @brief	カメラの上方向を取得。
	 */
	CVector3 GetUp()
	{
		return m_up;
	}
	/*!
	 * @brief	カメラの前方向を取得。
	 */
	CVector3 GetForword()
	{
		return m_forward;
	}
	/*!
	 * @brief	カメラの横方向を取得。
	 */
	CVector3 GetRight()
	{
		return m_right;
	}
	/*!
	 * @brief	視点を設定。
	 */
	void SetPosition( CVector3 pos )
	{
		m_position = pos;
	}
	/*!
	 * @brief	注視点を設定。
	 */
	void SetTarget( CVector3 target )
	{
		m_target = target;
	}
	/*!
	 * @brief	カメラの上方向を設定。
	 */
	void SetUp( CVector3 up )
	{
		m_up = up;
	}
	/*!
	 *   カメラの前方向を設定。
	 */
	void SetForward(CVector3 forword)
	{
		m_forward = forword;
	}
	/*!
	 *   カメラの横方向を設定。
	 */
	void SetRight(CVector3 right)
	{
		m_right = right;
	}
	/*!
	 * @brief	遠平面を設定。
	 */
	void SetFar( float f )
	{
		m_far = f;
	}
	/*!
	 * @brief	近平面を設定。
	 */
	void SetNear( float n )
	{
		m_near = n;
	}
	/*!
	 * @brief	画角を設定。
	 */
	void SetViewAngle( float angle )
	{
		m_viewAngle = angle;
	}
	/// <summary>
	/// 平行投影の幅を設定
	/// </summary>
	/// <remarks>
	/// EnUpdateProjMatrixFuncが
	/// enUpdateProjMatrixFunc_Orthoの時だけ使用される
	/// </remarks>
	/// <param name="h">幅</param> 
	void SetWidth(float w)
	{
		m_width = w;
	}
	/// <summary>
/// 平行投影の幅を設定
/// </summary>
/// <remarks>
/// EnUpdateProjMatrixFuncが
/// enUpdateProjMatrixFunc_Orthoの時だけ使用される
	/// <param name="h">高さ</param>
	void SetHeight(float h)
	{
		m_height = h;
	}

	/// <summary>
	/// 射影行列の計算の仕方を設定
	/// </summary>
	/// <param name="func">EnUpdateProjMatrixFuncを参照</param>
	void SetUpdateProjMatrixFunc(EnUpdateProjMatrixFunc func)
	{
		m_updateProjeMatrixFunc = func;
	}
private:
	CMatrix	m_viewMatrix = CMatrix::Identity();		//ビュー行列。
	CMatrix m_projMatrix = CMatrix::Identity();		//プロジェクション行列。
	CMatrix m_viewMatrixInv;                        //ビュー行列
	CVector3 m_target = CVector3::Zero();			//注視点。
	CVector3 m_position = CVector3::Zero();			//視点。
	CVector3 m_up = CVector3::Up();					//上方向。
	CVector3 m_forward = CVector3::Front();         //前方向。
	CVector3 m_right = CVector3::Right();           //横方向。   
	float m_viewAngle = CMath::DegToRad(60.0f);		//画角。
	float m_far = 10000.0f;							//遠い平面までの距離。
	float m_near = 1.0f;							//近平面までの距離。
	float m_width = 1280.0f;						//平行投影の幅。
	float m_height = 720.0f;						//平行投影の高さ
	//射影行列の作成方法。
	EnUpdateProjMatrixFunc m_updateProjeMatrixFunc = enUpdateProjMatrixFunc_Perspective;
};

extern Camera g_camera3D;		//!<3Dカメラ。
extern Camera g_camera2D;		//!<2Dカメラ。