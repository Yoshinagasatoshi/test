#pragma once
#include "gameobject/ysGameObject.h"

namespace prefab {
	/// <summary>
	/// 3D���f����\������@�\����񋟂���N���X
	/// IGameObject�p�����Ă���̂ŁAg_gomgr��newGo���g�p����K�v������܂�
	/// </summary>
	class CSkinModelRender : public IGameObject {
	public:

		CSkinModelRender(){}
		/// <summary>
		/// deletego�����s���ꂽ��Ă΂��
		/// </summary>
		void OnDestroy() override;

	/*	void Init(const wchar_t* filepath,
			CAnimationClip* animationClips = nullptr,
			int numAnimationClips = 0,
			EnFbxUpAxis fbxUpAxis = enFbxUpAxisZ,
			int maxInstance = 1);*/
	};
}