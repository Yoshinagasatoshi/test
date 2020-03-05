#include "stdafx.h"
#include "Player.h"
#include <iostream>
#include "gameObject/ysGameObjectManager.h"
#include "Enemy.h"
#include "Enemy_asigaru.h"
#include "Wepon_ghost.h"

const float posClearRange = 600.0f * 600.0f;	//クリア判定を行う範囲。
const float PLAYER_COLLIDER_HEIGHT = 100.0f;	//プレイヤーのカプセルコライダーの高さ。
const float PLAYER_COLLIDER_RADIUS = 60.0f;		//プレイヤーのカプセルコライダーの半径。

const float SpeedAmount = 1500.0f;				//平面の移動量
const float gravity = 200.0f;					//重力
const float JumpPower = 1200.0f;				//プレイヤーの飛ぶ力
const float standardPower = 200.0f;				//プレイヤーの敵吹き飛ばし力
const float limit = 2.0f;						//重力係数の上限
const int Timer_ZERO = 0;						//0になる。そのまま

Player::Player()
{
	CharaconInit();
	//cmoファイルの読み込み。
	m_playerModel.Init(L"Assets/modelData/busyo.cmo");
	//移動状態のロード
	m_busyoAnimeClip[animClip_idle].Load(L"Assets/animData/busyo_idle.tka");
	m_busyoAnimeClip[animClip_idle].SetLoopFlag(true);
	m_busyoAnimeClip[animClip_Walk].Load(L"Assets/animData/busyo_walk.tka");
	m_busyoAnimeClip[animClip_Walk].SetLoopFlag(true);
	//攻撃アニメロード
	m_busyoAnimeClip[animClip_ATK1].Load(L"Assets/animData/busyo_kougeki.tka");
	m_busyoAnimeClip[animClip_ATK2].Load(L"Assets/animData/busyo_kougeki2.tka");
	m_busyoAnimeClip[animClip_ATK3].Load(L"Assets/animData/busyo_kougeki3.tka");
	m_busyoAnimeClip[animClip_ATK4].Load(L"Assets/animData/busyo_kougeki4.tka");
	m_busyoAnimeClip[animClip_ATK5].Load(L"Assets/animData/busyo_kougeki5.tka");
	//ダメージロード
	m_busyoAnimeClip[animClip_SmallDamage].Load(L"Assets/animData/busyo_smalldamage.tka");
	m_busyoAnimeClip[animClip_busyo_dead].Load(L"Assets/animData/busyo_dead.tka");
	//全部一回の再生でいいのでfalseにしとく
	for (int i = animClip_ATK1; i < animClip_num; i++) {
		m_busyoAnimeClip[i].SetLoopFlag(false);
	}
	//アニメーション先モデルとアニメーションの個数を設定。
	m_busyoAnime.Init(
		m_playerModel,
		m_busyoAnimeClip,
		animClip_num
	);

	m_skelton = &m_playerModel.GetSkeleton();
	//このコードはボーンの配列を確認するために書いているコード。直接ゲームには関わってない。
	//とりあえず30個分入る配列を作成
	const wchar_t* bonename[30];

	for (int i = 0; i < 29; i++) {
		bonename[i] = m_skelton->GetBone(i)->GetName();

		if (i == 28)
		{
			bonename[i + 1] = L"end";
		}
	}

	m_busyoAnime.AddAnimationEventListener(	[&](const wchar_t* clipName, const wchar_t* eventName)
	{
		auto m_bone = m_skelton->GetBone(20);
		CVector3 bonepos;
		//y成分を除いたボーンをセット
		bonepos.Set(
			m_bone->GetWorldMatrix().m[3][0],
			0,
			m_bone->GetWorldMatrix().m[3][2]
		);
		//OnAnimationEvent(clipName,eventName);

		//ボーンのposとプレイヤーのposを足した場所
		m_calcPos = m_position;
		//ghostが半分埋まっていたので少し上に合わせる。
		m_calcPos.y += 70.0f;

		m_pl_Wepon = g_goMgr.NewGameObject<Wepon_ghost>("PL_Wepon");
		m_pl_Wepon->SetPosition(m_calcPos);
		m_pl_Wepon->GhostInit();
	}
	);
}


Player::~Player()
{
}

void Player::CharaconInit()
{
	//キャラコンの初期化
	m_characon.Init(
		PLAYER_COLLIDER_RADIUS,//コライダーの半径の長さ
		PLAYER_COLLIDER_HEIGHT,//コライダーの高さ
		m_position
	);
}

void Player::Update()
{
	if (m_busyoState != BusyoDead) {
		//地面ついてる？
		if (m_characon.IsOnGround()) {
			//攻撃するときのモーション
			AttackMove();
			//重力の重みを軽くする
			m_gravity_keisuu = 0.1f;
			//ジャンプしてた？
			if (m_Jumpfrag) {
				m_Jumpfrag = false;
			}
		}
		//ステートごとにの処理に後でする。
		if (m_busyoState != BusyoAttack) {
			if (g_pad->IsTrigger(enButtonA)) {
				if (!m_Jumpfrag) {
					m_moveSpeed.y += JumpPower;
					m_Jumpfrag = true;
					m_animStep = 0;
				}
			}
			m_gravity_keisuu += 0.1f;
			if (m_gravity_keisuu > limit) {
				m_gravity_keisuu = limit;
			}
			m_busyoAnime.Play(animClip_idle, 0.5f);
		}
		//ここら辺の処理ではほかに関数を使った方がいいかも
		if (m_damagefrag)
		{
			m_damagefrag = false;
			if (m_PL_HP != 0) {
				m_PL_HP--;
			}
			else {
				//m_deadFrag = true;
				m_busyoState = BusyoDead;
			}
			m_animState = animClip_SmallDamage;
			m_busyoAnime.Play(animClip_SmallDamage, 0.5f);
		}

		//ダメージアニメーションが終わったら立ち姿に
		if (!m_busyoAnime.IsPlaying())
		{
			m_animState = animClip_idle;
			m_busyoState = BusyoNormal;
			//if (m_underAttack) {
			//	m_underAttack = false;
			//}
			m_busyoAnime.Play(animClip_idle, 0.2f);
		}
		//移動処理
		Move();
		//回転処理
		Turn();
		//武器のゴーストが自分たちに当たったら、死んだという信号を立てる
		QueryGOs<Wepon_ghost>("EN_Wepon", [&](Wepon_ghost* wepon) {
			PhysicsGhostObject* ghostobject = wepon->GetGhostObject();
			g_physics.ContactTest(m_characon, [&](const btCollisionObject& contactObject) {
				if (ghostobject->IsSelf(contactObject) == true) {
					//通っているのは確認完了
					PlayerDamage();

					g_Effect.m_playEffectHandle = g_Effect.m_effekseerManager->Play(g_Effect.m_sampleEffect, m_position.x, m_position.y + 100.0f, m_position.z);
				}
				});
			return true;
			});
	}
	else {
		//プレイヤーが死んでいる時の処理
		m_moveSpeed = CVector3::Zero();
		m_busyoAnime.Play(animClip_busyo_dead);
	}
	Execute();
}

void Player::Move()
{
	//プレイヤーが死んでいない時の処理。
	//平面の移動量はアプデごとにリセットする。
	m_moveSpeed.x = 0.0f;
	m_moveSpeed.z = 0.0f;
	if (m_busyoState != BusyoAttack) {
		//入力量を受け取る
		WideMoveL = g_pad->GetLStickXF();
		heightMoveL = g_pad->GetLStickYF();

		//カメラの前方向と右方向を取得
		m_CameraForward = g_camera3D.GetForword();
		m_CameraRight = g_camera3D.GetRight();
		//Yの情報はいらないので0にし、前と右方向の単位とする。
		m_CameraForward.y = 0.0f;
		m_CameraForward.Normalize();
		m_CameraRight.y = 0.0f;
		m_CameraRight.Normalize();
		//攻撃中は自由に動かない時にする。
		//m_busyoState = BusyoAttack;
		if (!m_underAttack) {
			m_moveSpeed += m_CameraForward * heightMoveL * SpeedAmount;
			m_moveSpeed += m_CameraRight * WideMoveL * SpeedAmount;
		}
	}
	m_moveSpeed.y -= gravity * m_gravity_keisuu;
}

void Player::Draw()
{
	//プレイヤーを描写
	m_playerModel.Draw(
		g_camera3D.GetViewMatrix(), 
		g_camera3D.GetProjectionMatrix()
	);
}


void Player::Turn()
{
	if (m_busyoState != BusyoAttack) {

		if (fabsf(m_moveSpeed.x) <= 0.001f    //fabsfは絶対値。m_movespeed.x&m_movespeedzが
			&& fabsf(m_moveSpeed.z) <= 0.001f) {//0.001以下の時には何もしない。
			return;
		}
		else {
			float angle = atan2(m_moveSpeed.x, m_moveSpeed.z);
			m_rotation.SetRotation(CVector3::AxisY(), angle);
		}
	}
}

void Player::AttackMove()
{
	//補間時間
	float InterpolationTime = 0.2f;
	if (g_pad->IsTrigger(enButtonX)&&m_playTimer>3.0f) {
	//m_busyoState = BusyoAttack;
		if (!m_underAttack)
		{
			m_underAttack = true;
		}
		m_busyoState = BusyoAttack;
		//判定します。
		//ストラテジーパターン予備軍
		switch (m_animStep)
		{
		case animClip_idle:
			m_busyoAnime.Play(animClip_ATK1, InterpolationTime);
			m_busyoAnimeClip->GetKeyFramePtrListArray();
			//enmuの離れた位置にアタックがあるため、最初だけ+= animClip_ATK1を足す
			m_animStep += animClip_ATK1;
			m_blowOffPower = standardPower * 1.7f;
			break;
		case animClip_ATK1:
			m_busyoAnime.Play(animClip_ATK2, InterpolationTime);
			m_animStep++;
			break;
		case animClip_ATK2:
			m_busyoAnime.Play(animClip_ATK3, InterpolationTime);
			m_animStep++;
			break;
		case animClip_ATK3:
			m_busyoAnime.Play(animClip_ATK4, InterpolationTime);
			m_animStep++;
			m_blowOffPower = standardPower * 2.0f;
			break;
		case animClip_ATK4:
			m_busyoAnime.Play(animClip_ATK5, InterpolationTime);
			m_animStep++;
			m_blowOffPower = standardPower * 2.5f;
			break;
		}
	}
	m_playTimer++;
	if (m_animStep != 0) {
		if (m_animStep != m_oldAnimStep) {
			m_playTimer = Timer_ZERO;
			m_oldAnimStep = m_animStep;
		}
		//最後まで行くと隙をさらす時間を増やす
		if (m_animStep == animClip_ATK5) {
			//増やす処理
			m_TimerRelease = 30;
		}
		if (m_playTimer >= m_TimerRelease) {
			//一定の時間が過ぎたらアニメステート関係を初期化
			m_busyoState = BusyoNormal;
			m_TimerRelease = 20;
			m_animStep = animClip_idle;
			m_oldAnimStep = animClip_idle;
			m_playTimer = Timer_ZERO;
			m_busyoAnime.Play(animClip_idle, InterpolationTime*2.0f);
			m_underAttack = false;
		}
	}
}

void Player::Execute()
{
	
	if (m_pl_Wepon != nullptr) {
		m_pl_Wepon->SetPosition(m_position);
	}
	
	//ワールド座標の更新　こっちのskeletonUpdateをいじる
	auto footStep = m_busyoAnime.Update(1.0f / 30.0f);//ローカル座標の更新　こっちはいじらない
	//if (m_busyoState == BusyoAttack) {
	//攻撃中はフットステップの移動量を加算する。
	CMatrix mBias = CMatrix::Identity();
	mBias.MakeRotationX(CMath::PI * -0.5f);
	CMatrix rotMatrix;
	//回転行列を作成する。
	rotMatrix.MakeRotationFromQuaternion(m_rotation);
	rotMatrix.Mul(mBias, rotMatrix);
	rotMatrix.Mul(footStep);
	footStep *= 60.0f;
	m_moveSpeed += footStep;
	//}
	m_position = m_characon.Execute(1.0f / 60.0f, m_moveSpeed);
	//ワールド行列の更新。
	m_playerModel.UpdateWorldMatrix(m_position, m_rotation, m_scale);
}


int Player::RequestEnemyData(CVector3 pos,Enemy* enemy)
{
	for (int i = 0; i < DestinationNum; i++)
	{
		//一番最初にエネミーの空いている所に情報を入れる
		if (m_enemydata[i].position.y == 0.0f) {
			m_enemydata[i].position = pos;
			m_enemydata[i].enemy = enemy;
			return i;
		}
		else if (m_enemydata[i].enemy == enemy) {
			return i;
		}

		if (m_enemydata[i].position.y != 0.0f) {
			CVector3 kyori = m_enemydata[i].position - pos;
			if (kyori.LengthSq() > posClearRange) {
				m_enemydata[i].enemy = NULL;
				m_enemydata[i].position = CVector3{0.0f,0.0f,0.0f};
				return -1;
			}
		}
	}
	//nullでないかつ
	//距離が離れていたら
	//-1を戻り値にする
	return -1;
}