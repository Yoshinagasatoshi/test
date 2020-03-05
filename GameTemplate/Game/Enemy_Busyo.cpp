#include "stdafx.h"
#include "gameObject/ysGameObjectManager.h"
#include "Wepon_ghost.h"
#include "Enemy_Busyo.h"

const float power = 500.0f;

Enemy_Busyo::Enemy_Busyo()
{
	//モデルがぶっ壊れる？
	//とりあえず仮の箱、後で差し替える。。
	m_model.Init(L"Assets/modelData/enemy_busyo.cmo");
	m_rotation = CQuaternion::Identity();
	m_scale = CVector3::One();
	m_animClip[ATK].Load(L"Assets/animData/enemy_Busyo_Attack.tka");
	m_animClip[IDL].Load(L"Assets/animData/enemy_Busyo_Idle.tka");
	m_animClip[MOVE].Load(L"Assets/animData/enemy_Busyo_Inflate.tka");
	m_animClip[DAMAGE].Load(L"Assets/animData/enemy_Busyo_Damage.tka");
	m_animClip[DEATH].Load(L"Assets/animData/enemy_Busyo_Death.tka");

	m_enemy_BusyoAnime.Init(
		m_model,
		m_animClip,
		AnimationClip_Num
	);

	m_animClip[ATK].SetLoopFlag(false);
	m_animClip[MOVE].SetLoopFlag(false);
	m_animClip[IDL].SetLoopFlag(true);
	m_animClip[DAMAGE].SetLoopFlag(false);
	m_animClip[DEATH].SetLoopFlag(false);

	m_enemy_BusyoAnime.AddAnimationEventListener([&](const wchar_t* clipName, const wchar_t* eventName) {
		(void)clipName;
		m_en_Wepon = g_goMgr.NewGameObject<Wepon_ghost>("EN_Wepon");
		m_en_Wepon->SetPosition(m_position);
		}
	);
}

Enemy_Busyo::~Enemy_Busyo()
{

}

void Enemy_Busyo::Update()
{
	//キャラコンが入っていなかったら入れる。
	if (!m_charaConUse) {
		CharaconInit();
		m_charaConUse = true;
	}
	//m_stateジャッジ。この中でm_stateが割り振られる。
	StateJudge();

	switch (m_state)
	{
	case ATK:
		AttackMove();
		break;
	case MOVE:
		NormalMove();
		break;
	case IDL:
		IdleMove();
		break;
	case DAMAGE:
		ThisDelete();
	}
	//だめーｚしょり
	ThisDamage();

	//ワールド座標の更新　こっちのskeletonUpdateをいじる
	auto footStep = m_enemy_BusyoAnime.Update(1.0f / 30.0f);//ローカル座標の更新　こっちはいじらない
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

	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	m_enemy_BusyoAnime.Update(1/30.0f);
}

void Enemy_Busyo::CharaconInit()
{
	//キャラコンの初期化
	m_characon.Init(
		60.0f, //半径
		100.0f,//高さ
		m_position//位置
	);
}

void Enemy_Busyo::Draw()
{
	m_model.Draw(
		g_camera3D.GetViewMatrix(),
		g_camera3D.GetProjectionMatrix()
	);
}


void Enemy_Busyo::StateJudge()
{
	distance = m_player->GetPosition() - m_position;
	if (distance.LengthSq() < BattleRange) {
		m_state = ATK;
	}
	if (distance.LengthSq() > BattleRange
		&& distance.LengthSq() < VigilanceRange) {
		m_state = MOVE;
	}
	if (distance.LengthSq() > VigilanceRange) {
		m_state = IDL;
	}
	//ダメージを受けている時は他ののステートよりも優先
	if (m_isDeadfrag) {
		m_state = DAMAGE;
	}
}



//攻撃範囲に来たらここに入る。
void Enemy_Busyo::AttackMove()
{
	m_frameTimer += 1.0f;

	//攻撃中の動き
	m_moveSpeed = CVector3::Zero();
	//ワールド座標の更新
	m_moveSpeed.y += grabity;
	if (m_characon.IsOnGround()) {
		m_moveSpeed.y = 0.0f;
	}
	//攻撃の感覚を決める
	AttackframeNum();
	if (m_frameTimer > m_attackFrameNum)
	{
		m_enemy_BusyoAnime.Play(ATK, 0.2f);
		if (!m_enemy_BusyoAnime.IsPlaying())
		{
			m_frameTimer = 0;
			m_enemy_BusyoAnime.Play(MOVE, 0.2f);
		}
	}
	m_position = m_characon.Execute(1.0f / 30.0f, m_moveSpeed);
}

//索敵範囲に来たらここに入る
void Enemy_Busyo::NormalMove()
{
	CVector3 direction = distance;
	direction.Normalize();
	m_moveSpeed = direction * power;
	//ワールド座標の更新
	m_moveSpeed.y += grabity;
	if (m_characon.IsOnGround()) {
		m_moveSpeed.y = 0.0f;
	}

	m_enemy_BusyoAnime.Play(MOVE,0.2f);
	m_position = m_characon.Execute(1.0f / 30.0f, m_moveSpeed);
}

//距離が遠すぎたらここに入る。
void Enemy_Busyo::IdleMove()
{
	m_moveSpeed = CVector3::Zero();
	//ワールド座標の更新
	m_moveSpeed.y += grabity;
	if (m_characon.IsOnGround()) {
		m_moveSpeed.y = 0.0f;
	}

	m_enemy_BusyoAnime.Play(IDL, 0.2f);
	m_position = m_characon.Execute(1.0f / 30.0f, m_moveSpeed);
}

void Enemy_Busyo::ThisDamage()
{
	//武器のゴーストが自分たちに当たったら、死んだという信号を立てる
	QueryGOs<Wepon_ghost>("PL_Wepon", [&](Wepon_ghost* wepon) {
		PhysicsGhostObject* ghostobject = wepon->GetGhostObject();
		g_physics.ContactTest(m_characon, [&](const btCollisionObject& contactObject) {
			if (ghostobject->IsSelf(contactObject) == true) {

				//通っているのは確認完了
				m_isDeadfrag = true;

				//エフェクトも出す。
				g_Effect.m_playEffectHandle = g_Effect.m_effekseerManager->Play(
					g_Effect.m_sampleEffect,
					m_position.x, m_position.y + 100.0f,
					m_position.z
				);

			}
			});
		return true;
		});
}

void Enemy_Busyo::ThisDelete()
{
	//体力はまだ残っているかどうか
	if (m_HP != 0) {
		m_HP--;
		m_enemy_BusyoAnime.Play(DAMAGE, 0.2f);
		m_isDeadfrag = false;
	}
	//なくなっていたらこちらを通る
	else {
		m_enemy_BusyoAnime.Play(DEATH, 0.1f);
		if (!m_enemy_BusyoAnime.IsPlaying()) {
			DeleteGO(this);
		}
	}
}
