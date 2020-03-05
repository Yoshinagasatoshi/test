/*!
 *@brief	アニメーションクリップ。
 */

#pragma once

//////////////////////////////////////////////////////////////////////
// 各種構造体
//////////////////////////////////////////////////////////////////////
/*!
*@brief	アニメーションクリップのヘッダー。
*/
struct AnimClipHeader {
	std::uint32_t numKey;				//!<キーフレームの数。
	std::uint32_t numAnimationEvent;	//!<アニメーションイベントの数。
};
/*!
*@brief	アニメーションイベント
*@アニメーションイベントは未対応。
* やりたかったら自分で実装するように。
*/
struct AnimationEventData {
	float	invokeTime;					//!<アニメーションイベントが発生する時間(単位:秒)
	std::uint32_t eventNameLength;		//!<イベント名の長さ。
};
/*!
*@brief	キーフレーム。
*/
struct Keyframe {
	std::uint32_t boneIndex;	//!<ボーンインデックス。
	float time;					//!<時間。
	CMatrix transform;			//!<トランスフォーム。
};
/*!
*@brief	キーフレーム。
*/
struct KeyframeRow {
	std::uint32_t boneIndex;	//!<ボーンインデックス。
	float time;					//!<時間。
	CVector3 transform[4];		//!<トランスフォーム。
};
/*
アニメーションイベント
*/
class CAnimationEvent
{
public:
	CAnimationEvent()
	{
	}
	/// <summary>
	///イベント発生時間を取得
	/// </summary>
	/// <returns></returns>
	float GetInvokeTime() const
	{
		return m_invokeTime;
	}
	/// <summary>
	/// イベント名を取得
	/// </summary>
	/// <returns></returns>
	const wchar_t* GetEventName() const
	{
		return m_evemtName.c_str();
	}
	/// <summary>
	/// イベント発生時間を設定
	/// </summary>
	/// <param name="time"></param>
	void SetInvokeTime(float time)
	{
		m_invokeTime = time;
	}
	/*
	イベント名を設定
	*/
	void SetEventName(const wchar_t* name)
	{
		m_evemtName = name;
	}
	/// <summary>
	/// イベントは発生している？
	/// </summary>
	/// <returns></returns>
	bool IsInvoked() const
	{
		return m_isInvoked;
	}
	/// <summary>
	/// 呼ばれたらイベント発生済みフラグを立てる。
	/// </summary>
	/// <param name="flag"></param>
	void SetInvokedFlag(bool flag)
	{
		m_isInvoked = flag;
	}
private:
	bool m_isInvoked = false;		//イベントは発生済みか？
	float m_invokeTime;				//イベント発生時間
	std::wstring m_evemtName;		//イベントの名前
};
/*
*@brief	アニメーションクリップ。
*/
class AnimationClip  {
public:
	//タイプ量が長ったらしくて、うざいのでstd::vector<KeyFrame*>の別名定義。
	using keyFramePtrList = std::vector<Keyframe*>;
	/*!
	* @brief	コンストラクタ
	*/
	AnimationClip() 
	{
	}
	/*!
	*@brief	デストラクタ。
	*/
	~AnimationClip();
	/*!
	*@brief	アニメーションクリップをロード。
	*@param[in]	filePath	ファイルパス。
	*/
	void Load(const wchar_t* filePath,const wchar_t* clipName = nullptr);

	/*!
	*@brief	ループする？
	*/
	bool IsLoop() const
	{
		return m_isLoop;
	}
	/*!
	*@brief	ループフラグを設定する。
	*/
	void SetLoopFlag(bool flag)
	{
		m_isLoop = flag;
	}
	/*!
	*@brief
	*/
	const std::vector<keyFramePtrList>& GetKeyFramePtrListArray() const
	{
		return m_keyFramePtrListArray;
	}
	const keyFramePtrList& GetTopBoneKeyFrameList() const
	{
		return *m_topBoneKeyFramList;
	}
	/// <summary>
	/// クリップ名を取得
	/// </summary>
	const wchar_t* GetName() const
	{
		return m_clipName.c_str();
	}
	/// <summary>
	/// アニメーションイベントを取得
	/// </summary>
	std::unique_ptr<CAnimationEvent[]>& GetAnimationEvent()
	{
		return m_animationEvent;
	}
	/// <summary>
	/// アニメーションイベントの数を取得
	/// </summary>
	int GetNumAnimationEvent() const
	{
		return m_numAnimationEvent;
	}
private:
	
	std::wstring m_clipName;								//アニメーションクリップの名前
	bool m_isLoop = false;									//!<ループフラグ。
	std::vector<Keyframe*> m_keyframes;						//全てのキーフレーム。
	std::vector<keyFramePtrList> m_keyFramePtrListArray;	//ボーンごとのキーフレームのリストを管理するための配列。
															//例えば、m_keyFramePtrListArray[0]は0番目のボーンのキーフレームのリスト、
															//m_keyFramePtrListArray[1]は1番目のボーンのキーフレームのリストといった感じ。
	std::unique_ptr<CAnimationEvent[]> m_animationEvent;	//アニメーションイベント
	int	 m_numAnimationEvent = 0;							//アニメーションイベントの数
	keyFramePtrList* m_topBoneKeyFramList = nullptr;
	bool m_loaded = false; //アニメーションクリップがロードされている
};

