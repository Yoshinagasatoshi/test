/*!
 * @brief	アニメータークラス。
 */


#include "stdafx.h"
#include "graphics/animation/Animation.h"
#include "graphics/skeleton.h"
#include "graphics/skinModel.h"

Animation::Animation()
{
}
Animation::~Animation()
{
	
}
	
void Animation::Init(SkinModel& skinModel, AnimationClip animClipList[], int numAnimClip)
{
	if (animClipList == nullptr) {
#ifdef _DEBUG
		char message[256];
		strcpy(message, "animClipListがNULLです。\n");
		OutputDebugStringA(message);
		//止める。
		std::abort();
#endif
		
	}
	m_skeleton = &skinModel.GetSkeleton();
	//footstepボーンの番号を調べる。
	int numBone = m_skeleton->GetNumBones();
	for (int boneNo = 0; boneNo < numBone; boneNo++) {
		auto bone = m_skeleton->GetBone(boneNo);
		if (wcscmp(bone->GetName(), L"footstep") == 0) {
			//footstepボーンが見つかった。
			m_footStepBoneNo = boneNo;
			break;
		}
	}
	for (int i = 0; i < numAnimClip; i++) {
		m_animationClips.push_back(&animClipList[i]);
	}
	for (auto& ctr : m_animationPlayController) {
		ctr.Init(m_skeleton, m_footStepBoneNo);
	}
		
	Play(0);
}
/*!
* @brief	ローカルポーズの更新。
*/
void Animation::UpdateLocalPose(float deltaTime)
{
	m_interpolateTime += deltaTime;
	if (m_interpolateTime >= 1.0f) {
		//補間完了。
		//現在の最終アニメーションコントローラへのインデックスが開始インデックスになる。
		m_startAnimationPlayController = GetLastAnimationControllerIndex();
		m_numAnimationPlayController = 1;
		m_interpolateTime = 1.0f;
	}
	//AnimationPlayController::Update関数を実行していく。
	for (int i = 0; i < m_numAnimationPlayController; i++) {
		int index = GetAnimationControllerIndex(m_startAnimationPlayController, i );
		m_animationPlayController[index].Update(deltaTime, this);
	}
}

CVector3 Animation::UpdateGlobalPose()
{
	//グローバルポーズ計算用のメモリをスタックから確保。
	int numBone = m_skeleton->GetNumBones();
	CQuaternion* qGlobalPose = (CQuaternion*)alloca(sizeof(CQuaternion) * numBone);
	CVector3* vGlobalPose = (CVector3*)alloca(sizeof(CVector3) * numBone);
	CVector3* vGlobalScale = (CVector3*)alloca(sizeof(CVector3) * numBone);
	CVector3 vGlobalDeltaFootStep = CVector3::Zero();

	for (int i = 0; i < numBone; i++) {
		qGlobalPose[i] = CQuaternion::Identity();
		vGlobalPose[i] = CVector3::Zero();
		vGlobalScale[i] = CVector3::One();
	}
	//グローバルポーズを計算していく。
	int startIndex = m_startAnimationPlayController;
	for (int i = 0; i < m_numAnimationPlayController; i++) {
		int index = GetAnimationControllerIndex(startIndex, i);
		float intepolateRate = m_animationPlayController[index].GetInterpolateRate();
		const auto& localBoneMatrix = m_animationPlayController[index].GetBoneLocalMatrix();
		auto deltaValueFootStep = m_animationPlayController[index].GetDeltaValueFootstepBone();
		vGlobalDeltaFootStep.Lerp(intepolateRate, vGlobalDeltaFootStep, deltaValueFootStep);
		for (int boneNo = 0; boneNo < numBone; boneNo++) {
			//平行移動の補完
			CMatrix m = localBoneMatrix[boneNo];
			vGlobalPose[boneNo].Lerp(
				intepolateRate, 
				vGlobalPose[boneNo], 
				*(CVector3*)m.m[3]
			);
			//平行移動成分を削除。
			m.m[3][0] = 0.0f;
			m.m[3][1] = 0.0f;
			m.m[3][2] = 0.0f;
			
			//拡大成分の補間。
			CVector3 vBoneScale;
			vBoneScale.x = (*(CVector3*)m.m[0]).Length();
			vBoneScale.y = (*(CVector3*)m.m[1]).Length();
			vBoneScale.z = (*(CVector3*)m.m[2]).Length();

			vGlobalScale[boneNo].Lerp(
				intepolateRate,
				vGlobalScale[boneNo],
				vBoneScale
			);
			//拡大成分を除去。
			m.m[0][0] /= vBoneScale.x;
			m.m[0][1] /= vBoneScale.x;
			m.m[0][2] /= vBoneScale.x;

			m.m[1][0] /= vBoneScale.y;
			m.m[1][1] /= vBoneScale.y;
			m.m[1][2] /= vBoneScale.y;

			m.m[2][0] /= vBoneScale.z;
			m.m[2][1] /= vBoneScale.z;
			m.m[2][2] /= vBoneScale.z;
				
			//回転の補完
			CQuaternion qBone;
			qBone.SetRotation(m);
			qGlobalPose[boneNo].Slerp(intepolateRate, qGlobalPose[boneNo], qBone);
		}
	}
	//グローバルポーズをスケルトンに反映させていく。
	for (int boneNo = 0; boneNo < numBone; boneNo++) {
		
		//拡大行列を作成。
		CMatrix scaleMatrix;
		scaleMatrix.MakeScaling(vGlobalScale[boneNo]);
		//回転行列を作成。
		CMatrix rotMatrix;
		rotMatrix.MakeRotationFromQuaternion(qGlobalPose[boneNo]);
		//平行移動行列を作成。
		CMatrix transMat;
		transMat.MakeTranslation(vGlobalPose[boneNo]);

		//全部を合成して、ボーン行列を作成。
		CMatrix boneMatrix;
		boneMatrix.Mul(scaleMatrix, rotMatrix);
		boneMatrix.Mul(boneMatrix, transMat);
		
		m_skeleton->SetBoneLocalMatrix(
			boneNo,
			boneMatrix
		);			
	}
		
	//最終アニメーション以外は補間完了していたら除去していく。
	int numAnimationPlayController = m_numAnimationPlayController;
	for (int i = 1; i < m_numAnimationPlayController; i++) {
		int index = GetAnimationControllerIndex(startIndex, i);
		if (m_animationPlayController[index].GetInterpolateRate() > 0.99999f) {
			//補間が終わっているのでアニメーションの開始位置を前にする。
			m_startAnimationPlayController = index;
			numAnimationPlayController = m_numAnimationPlayController - i;
		}
	}
	m_numAnimationPlayController = numAnimationPlayController;
	return vGlobalDeltaFootStep;
}
	

	
CVector3 Animation::Update(float deltaTime)
{
	if (m_numAnimationPlayController == 0) {
		return CVector3::Zero();
	}
	//ローカルポーズの更新をやっていく。
	UpdateLocalPose(deltaTime);
		
	//グローバルポーズを計算していく。
	return UpdateGlobalPose();
}
