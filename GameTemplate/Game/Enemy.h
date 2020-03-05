#pragma once
#include "gameObject/ysGameObject.h"
#include "Player.h"
/// <summary>
/// 敵兵クラス。
/// <remarks>
/// 敵の基本的な動きを与えるクラスです
/// 足軽や敵将クラスの基底クラスになる予定
/// </remarks>
/// </summary>
class Player;
class GameCamera;
class Enemy : public IGameObject
{
public:
	Enemy();
	virtual ~Enemy();
	//virtual void Update();
	//virtual void Draw();
	//virtual void Move() = 0;
	CVector3 SetPos(const CVector3 position)
	{
		return m_position = position;
	}
	CQuaternion SetRot(const CQuaternion qrot)
	{
		return m_rotation = qrot;
	}
	void SetPlayerInfo(Player* pl)
	{
		m_player = pl;
	}
	void SetGameCameraInfo(GameCamera* camera)
	{
		m_gameCamera = camera;
	}
protected:
	SkinModel m_model;			//スキンモデル。
	SkinModel m_model_Row;		//ローポリのスキンモデル。アニメはつけない予定
	CVector3 m_position;		//現在地
	CQuaternion m_rotation;		//回転角度
	CVector3 m_scale;
	CVector3 m_playerPos;		//プレイヤーのポジション
	Player* m_player;			//プレイヤーのポイント型
	GameCamera* m_gameCamera;	//ゲームカメラのポイント型。
	const float grabity = -9.8f * 2.0f;//重力
	const float BattleRange = 180.0f * 180.0f;			//この距離の範囲内に近づくとバトル
	const float VigilanceRange = 600.0f * 600.0f;		//この距離の範囲内ならプレイヤーに近づく。
};

