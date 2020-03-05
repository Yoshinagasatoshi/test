#pragma once
#include "gameobject/ysGameObject.h"

namespace prefab {
	/// <summary>
	/// 3Dモデルを表示する機能えを提供するクラス
	/// IGameObject継承しているので、g_gomgrのnewGoを使用する必要があります
	/// </summary>
	class CSkinModelRender : public IGameObject {
	public:

		CSkinModelRender(){}
		/// <summary>
		/// deletegoが実行されたら呼ばれる
		/// </summary>
		void OnDestroy() override;

	/*	void Init(const wchar_t* filepath,
			CAnimationClip* animationClips = nullptr,
			int numAnimationClips = 0,
			EnFbxUpAxis fbxUpAxis = enFbxUpAxisZ,
			int maxInstance = 1);*/
	};
}