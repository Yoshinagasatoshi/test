/*!
 * @brief	アニメーション再生コントローラ。
 */

#include "stdafx.h"
#include "graphics/animation/AnimationPlayController.h"
#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
#include "graphics/skeleton.h"


	
	
void AnimationPlayController::Init(Skeleton* skeleton, int footStepBoneNo)
{
	m_footstepBoneNo = footStepBoneNo;
	int numBones = skeleton->GetNumBones();
	//ボーン行列をバシッと確保。
	m_boneMatrix.resize(numBones);
	m_skelton = skeleton;
}
	
void AnimationPlayController::InvokeAnimationEvent(Animation* animation)
{
	auto& animEventArray = m_animationClip->GetAnimationEvent();
	for (auto i = 0; i < m_animationClip->GetNumAnimationEvent(); i++) {
		if (m_time > animEventArray[i].GetInvokeTime()
			&& animEventArray[i].IsInvoked() == false) {
			//アニメーションの起動時間を過ぎているかつ、イベントがまだ起動していない。
			animation->NotifyAnimationEventToListener(
				m_animationClip->GetName(), animEventArray[i].GetEventName()
			);
			animEventArray[i].SetInvokedFlag(true);
		}
	}
}
	
void AnimationPlayController::StartLoop()
{
	m_currentKeyFrameNo = 0;
	m_time = 0.0f;
	//アニメーションイベントを全て未発生にする。
	auto& animEventArray = m_animationClip->GetAnimationEvent();
	for (auto i = 0; i < m_animationClip->GetNumAnimationEvent(); i++) {
		animEventArray[i].SetInvokedFlag(false);
	}
}
void AnimationPlayController::UpdateBoneWorldMatrix(Bone& bone, const CMatrix& parentMatrix)
{
	//ワールド行列を計算する。
	auto& mBoneWorld = m_boneMatrix[bone.GetNo()];
	CMatrix localMatrix = m_boneMatrix[bone.GetNo()];
	//親の行列とローカル行列を乗算して、ワールド行列を計算する。
	mBoneWorld.Mul(localMatrix, parentMatrix);
	
	//子供のワールド行列も計算する。
	std::vector<Bone*>& children = bone.GetChildren();
	for (int childNo = 0; childNo < children.size(); childNo++) {
		//この骨のワールド行列をUpdateBoneWorldMatrixに渡して、さらに子供のワールド行列を計算する。
		UpdateBoneWorldMatrix(*children[childNo], mBoneWorld);
	}
}
void AnimationPlayController::Update(float deltaTime, Animation* animation)
{
	if(m_animationClip == nullptr){
		
		return ;
	}
	const auto& topBoneKeyFrameList = m_animationClip->GetTopBoneKeyFrameList();
	m_time += deltaTime;

	//アニメーションイベントの発生
	InvokeAnimationEvent(animation);
	//補完時間も進めていく。
	m_interpolateTime = min(1.0f, m_interpolateTime + deltaTime);
	while (true) {
		if (m_currentKeyFrameNo >= (int)topBoneKeyFrameList.size()) {
			//終端まで行った。
			if (m_animationClip->IsLoop()) {
				//ループ。
				StartLoop();
			}
			else {
				//ワンショット再生。
				m_currentKeyFrameNo--;
				m_isPlaying = false;	//再生終わり。
			}
			break;
		}
		if (topBoneKeyFrameList.at(m_currentKeyFrameNo)->time >= m_time) {
			//終わり。
			break;
		}
		//次へ。
		m_currentKeyFrameNo++;
	}
	//ボーン行列を計算していく。
	const auto& keyFramePtrListArray = m_animationClip->GetKeyFramePtrListArray();
	for (const auto& keyFrameList : keyFramePtrListArray) {
		if (keyFrameList.size() == 0) {
			continue;
		}
		//現在再生中のキーフレームを取ってくる。
		Keyframe* keyframe = keyFrameList.at(m_currentKeyFrameNo);
		if (keyframe->boneIndex < m_boneMatrix.size()) {
			m_boneMatrix[keyframe->boneIndex] = keyframe->transform;
		}
		else {
#ifdef _DEBUG			
			MessageBox(NULL, "AnimationPlayController::Update : 存在しないボーンに値を書き込もうとしています。次のような原因が考えられます。\n"
				"① tkaファイルを出力する時に、選択したルートボーンがスケルトンのルートボーンと異なっている。\n"
				"② 異なるスケルトンのアニメーションクリップを使っている。\n"
				"もう一度tkaファイルを出力しなおしてください。", "error", MB_OK);
			std::abort();
#endif
		}
	}
	//スケルトン空間の行列を計算する。
	int numBone = m_skelton->GetNumBones();
	for (int boneNo = 0; boneNo < numBone; boneNo++) {
		//親の骨を検索する。
		auto bone = m_skelton->GetBone(boneNo);
		if (bone->GetParentId() != -1) {
			continue;
		}
		UpdateBoneWorldMatrix(*bone, CMatrix::Identity());
	}

	for (int boneNo = 0; boneNo < numBone; boneNo++) {
		auto bone = m_skelton->GetBone(boneNo);
		if (m_footstepBoneNo == bone->GetNo()) {
			
			auto deltaValueFootStepBoneLastFrame = m_deltaValueFootstepBone;
			auto mat = m_boneMatrix[bone->GetNo()];

			m_deltaValueFootstepBone.x = mat.m[3][0];
			m_deltaValueFootstepBone.y = mat.m[3][1];
			m_deltaValueFootstepBone.z = mat.m[3][2];

			if (m_isFirst == true) {
				m_deltaValueFootstepBoneOneFrame = CVector3::Zero();
				m_isFirst = false;
			}
			else {
				m_deltaValueFootstepBoneOneFrame = m_deltaValueFootstepBone - deltaValueFootStepBoneLastFrame;
			}
		}
	}
	//footstepボーンのxz平面での移動量を減算。
	for (int boneNo = 0; boneNo < numBone; boneNo++) {
		auto bone = m_skelton->GetBone(boneNo);
		m_boneMatrix[bone->GetNo()].m[3][0] -= m_deltaValueFootstepBone.x;
		m_boneMatrix[bone->GetNo()].m[3][1] -= m_deltaValueFootstepBone.y;
		m_boneMatrix[bone->GetNo()].m[3][2] -= m_deltaValueFootstepBone.z;
	}
}

