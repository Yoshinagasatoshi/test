#include "stdafx.h"
#include "Enemy_asigaru.h"
#include "gameObject/ysGameObjectManager.h"
#include "GameData.h"
#include "Wepon_ghost.h"
#include "GameCamera.h"
#include "math/Matrix.h"


const float tikadukisugi = 2.0f;
const float Timer_ZERO = 0.0f;
const float DeleteTime = 10.0f;
const float drawNearSpeed = 50.0f;
const float ViewLenght = 0.25f;						//視野角範囲

/// <summary>
/// boid
/// </summary>
Enemy_asigaru::IdlePos Enemy_asigaru::m_idlePos[5];
//コンストラクタ
Enemy_asigaru::Enemy_asigaru()
{
	gamedata = &GameData::GetInstans();
	m_scale = CVector3::One();

	//asigaruのモデルをロードする。
	m_model.Init(L"Assets/modelData/asigaru.cmo");
	m_model_Row.Init(L"Assets/modelData/asigaru_Rowpori.cmo");
	m_rotation = CQuaternion::Identity();
	//アニメーションを格納だ
	m_asigaruAnimeClip[Asigaru_totugeki].Load(L"Assets/animData/asigaru_totugeki.tka");
	m_asigaruAnimeClip[Asigaru_tikazuki].Load(L"Assets/animData/asigaru_tikazuki.tka");
	m_asigaruAnimeClip[Asigaru_sentou].Load(L"Assets/animData/asigaru_tikazuki.tka");
	m_asigaruAnimeClip[Asigaru_attack].Load(L"Assets/animData/asigaru_Normal_Attack.tka");
	m_asigaruAnimeClip[Asigaru_damage].Load(L"Assets/animData/asigaru_damage.tka");
	m_asigaruAnimeClip[Asigaru_dead].Load(L"Assets/animData/asigaru_dead.tka");

	m_asigaruAnimeClip[Asigaru_totugeki].SetLoopFlag(true);
	m_asigaruAnimeClip[Asigaru_tikazuki].SetLoopFlag(true);
	m_asigaruAnimeClip[Asigaru_sentou].SetLoopFlag(false);
	m_asigaruAnimeClip[Asigaru_damage].SetLoopFlag(false);
	m_asigaruAnimeClip[Asigaru_dead].SetLoopFlag(false);

	//モデルとアニメデータを初期化
	m_asigaruAnime.Init(
		m_model,
		m_asigaruAnimeClip,
		Asigaru_anim_num
	);
	//下のコードはskeletonの名前を検索している。
	m_skeleton = &m_model.GetSkeleton();
	const wchar_t* bonename[30];

	for (int i = 0; i < 20; i++) {
		bonename[i] = m_skeleton->GetBone(i)->GetName();

		if (i == 19)
		{
			bonename[i + 1] = L"end";
		}
	}

	//ghostInit();

	m_asigaruAnime.AddAnimationEventListener([&](const wchar_t* clipName, const wchar_t* eventName) {
		(void)clipName;
		m_en_Wepon = g_goMgr.NewGameObject<Wepon_ghost>("EN_Wepon");
		m_en_Wepon->SetPosition(m_position);
		}
	);
	g_goMgr.InitEffekseer();
	g_Effect.m_sampleEffect = Effekseer::Effect::Create(g_Effect.m_effekseerManager, (const EFK_CHAR*)L"Assets/effect/sword.efk");
	//エフェクトを再生する。
	g_Effect.m_playEffectHandle = g_Effect.m_effekseerManager->Play(g_Effect.m_sampleEffect, 0.0f, 0.0f, 0.0f);
}
//デストラクタ
Enemy_asigaru::~Enemy_asigaru()
{
	g_goMgr.DeleteGOObject(this);
	g_goMgr.Counting();
	//if (g_Effect.m_sampleEffect != nullptr)
	//{
	//	g_Effect.m_sampleEffect->Release();
	//}
}

void Enemy_asigaru::CharaconInit()
{
	//キャラコンの初期化
	m_characon.Init(
		60.0f, //半径
		100.0f,//高さ
		m_position//位置
	);
}

void Enemy_asigaru::Update()
{
	//Y成分の移動速度をバックアップしておく。
	float ySpeed = m_moveSpeed.y;

	//キャラコンが作られていなかったらinitする
	if (!m_characonConUse) {
		m_characonConUse = true;
		CharaconInit();
	}

	//打ちあがって着地したときに動いてしまうので
	//無理やり動きを固定する
	if (m_isDeadfrag
		&&m_characon.IsOnGround()){
		m_moveSpeed = CVector3::Zero();
		m_Deathtimer_f++;
		//死んだ後に少したってモデルが消える
		if (m_Deathtimer_f > DeleteTime) {
			//m_characon.RemoveRigidBoby();
			DeleteGO(this);
		}
	}

	//死んでいるかどうかで行う処理が変わる
	if (m_isDeadfrag
		&&!m_deadMoveStopper) {
		m_deadMoveStopper = true;
		DeadMove();
		ySpeed = m_moveSpeed.y;
	}
	else if(!m_isDeadfrag){

		//回転処理。プレイヤーの方に向くだけ
		Turn();
		StateJudge();
	}


	//武器のゴーストが自分たちに当たったら、死んだという信号を立てる
	QueryGOs<Wepon_ghost>("PL_Wepon", [&](Wepon_ghost* wepon) {
		PhysicsGhostObject* ghostobject = wepon->GetGhostObject();
		g_physics.ContactTest(m_characon, [&](const btCollisionObject& contactObject) {
		if (ghostobject->IsSelf(contactObject) == true) {
			//通っているのは確認完了
			m_isDeadfrag = true;

			g_Effect.m_playEffectHandle = g_Effect.m_effekseerManager->Play(g_Effect.m_sampleEffect, m_position.x, m_position.y + 100.0f, m_position.z);
		}
		});
		return true;
	});

	//ワールド座標の更新
	m_moveSpeed.y = ySpeed + grabity;

	//m_position += m_moveSpeed;
	m_position = m_characon.Execute(1.0f / 30.0f, m_moveSpeed);
	m_model.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	m_model_Row.UpdateWorldMatrix(m_position, m_rotation, m_scale);
	//Effekseerカメラ行列を設定。
	//まずはEffeseerの行列型の変数に、カメラ行列とプロジェクション行列をコピー。
	Effekseer::Matrix44 efCameraMat;
	g_camera3D.GetViewMatrix().CopyTo(efCameraMat);
	Effekseer::Matrix44 efProjMat;
	g_camera3D.GetProjectionMatrix().CopyTo(efProjMat);
	//カメラ行列とプロジェクション行列を設定。
	g_Effect.m_effekseerRenderer->SetCameraMatrix(efCameraMat);
	g_Effect.m_effekseerRenderer->SetProjectionMatrix(efProjMat);
	//m_ghostObject.SetPosition(m_position);
	m_asigaruAnime.Update(1.0f / 30.0f);
	g_Effect.m_effekseerManager->Update();
}
void Enemy_asigaru::Draw()
{

	CVector3 cameraPos = m_gameCamera->GetCameraPos();
	CVector3 Lenght = cameraPos - m_position;
	Lenght.y = 0.0f;
	//カメラとの距離を測って、距離が遠すぎたらローポリに切り替えるようにする
	if (Lenght.LengthSq() <2500.0f * 2500.0f) {
		//モデルの描画
		m_model.Draw(
			g_camera3D.GetViewMatrix(),
			g_camera3D.GetProjectionMatrix()
		);
	}
	else {
		m_model_Row.Draw(
			g_camera3D.GetViewMatrix(),
			g_camera3D.GetProjectionMatrix()
		);
	}
}

//ここが　取り巻く処理を書いている場所
void Enemy_asigaru::Move()
{
	const int NomalMove = -1;
	int kakoi_num = m_player->RequestEnemyData(m_position, this);
	int a = kakoi_num;
	//周辺を取り巻くやつらなのか？
	if (kakoi_num != NomalMove) {
		//ここから下は取り巻くやつの処理
		CVector3 kaiten = m_playerPos - m_position;
		//ここだけ動きと回転の処理を変える
		idlePosInit();
		m_moveSpeed = m_idlePos[kakoi_num].idlePos - m_position;
		//目的地と距離があまりにも近いときは動かない
		if (m_moveSpeed.Length() < 5.0f) {
			m_moveSpeed = CVector3::Zero();
		}
		else {
			m_moveSpeed.Normalize();
			m_moveSpeed *= 30.0f;//30倍
		}
	float angle = atan2(kaiten.x,kaiten.z);
	m_rotation.SetRotation(CVector3::AxisY(), angle);
	return;
	}
	else {
		CVector3 kaiten = m_playerPos - m_position;
		float angle = atan2(kaiten.x, kaiten.z);
		m_rotation.SetRotation(CVector3::AxisY(), angle);

		if (kaiten.LengthSq() < BattleRange) {
			int i = 0;
			i = rand() % 5;
			CVector3 panko = CVector3::Zero();
			panko = m_idlePos[i].idlePos - m_position;
			panko.Normalize();
			m_moveSpeed = CVector3::Zero();
		}

	}
}

void Enemy_asigaru::Turn()
{
	auto Rot = CMatrix::Identity();
	Rot.MakeRotationFromQuaternion(m_rotation);
	m_forward.x = Rot.m[2][0];
	m_forward.y = Rot.m[2][1];
	m_forward.z = Rot.m[2][2];

	if (fabsf(m_moveSpeed.x) <= 0.001f    //fabsfは絶対値。m_movespeed.x&m_movespeedzが
		&& fabsf(m_moveSpeed.z) <= 0.001f) {//0.001以下の時には何もしない。
		return;
	}
	else {
		float angle = atan2(m_moveSpeed.x, m_moveSpeed.z);
		m_rotation.SetRotation(CVector3::AxisY(), angle);
	}
}
//周りを囲むプレイヤーの周りを囲む場所の情報
void Enemy_asigaru::idlePosInit()
{
	//４点を作ってそこにエネミーを群がらせる
	//レベルでやった方がいいというアドヴァイスをもらったので
	//後でレベルに変える。
	m_playerPos = m_player->GetPosition();

	m_idlePos[0].idlePos = m_playerPos + CVector3{150.0f,0.0f,0.0f};
	m_idlePos[1].idlePos = m_playerPos + CVector3{ -150.0f,0.0f,0.0f };
	m_idlePos[2].idlePos = m_playerPos + CVector3{ 100.0f,0.0f,100.0f };
	m_idlePos[3].idlePos = m_playerPos + CVector3{ -100.0f,0.0f,100.0f };
	m_idlePos[4].idlePos = m_playerPos + CVector3{ 0.0f,0.0f,-150.0f };
}

//距離による判定処理
void Enemy_asigaru::StateJudge()
{
	//～～Range
	m_playerPos = m_player->GetPosition();
	CVector3 kyori = m_playerPos - m_position;
	//y成分を0にする。
	kyori.y = 0.0f;
	
	m_player_isdead = m_player->GetPlayerDead();
	//ステートごとの処理を書く
	switch (m_asigaruState)
	{
	case Asigaru_attack:
		Move();
		m_moveSpeed = CVector3::Zero();
		if (!m_asigaruAnime.IsPlaying()) {
			m_asigaruState = Asigaru_sentou;
		}
		break;
	case Asigaru_sentou:
		if (m_isAttack) {
			m_isAttack = false;
		}
		if (kyori.LengthSq() > BattleRange) {
			m_asigaruState = Asigaru_tikazuki;
			m_frameTimer = Timer_ZERO;
			AttackframeNum();
		}
		//sentouの処理
		m_moveSpeed = CVector3::Zero();
		m_frameTimer += 1.0f;

		if (m_frameTimer >= m_kougekiframenum)
		{
			m_frameTimer = Timer_ZERO;
			AttackframeNum();

			////距離判定が近かったら殴られたときにダメージ
			//CVector3 distans = m_position - m_player->GetPosition();
			//if (distans.LengthSq() < 100.0f * 100.0f) {
			//	m_player->PlayerDamage();
			//}
			m_asigaruState = Asigaru_attack;
		}
		Move();
		break;
	case Asigaru_tikazuki:
		if (kyori.LengthSq() < BattleRange) {
			kyori.Normalize();
			float angle = acosf(kyori.Dot(m_forward));

			//視野角入ってる？
			if (fabsf(angle) < PI * ViewLenght) {
				m_asigaruState = Asigaru_sentou;
			}
		}
		else if (kyori.LengthSq() > VigilanceRange)
		{
			m_asigaruState = Asigaru_totugeki;
		}
		//tikazukiの処理
		kyori.Normalize();
		m_moveSpeed = kyori * drawNearSpeed;
		//Move();
		break;


	case Asigaru_totugeki:
		if (kyori.LengthSq() < VigilanceRange) {
			//視野角入ってる？
			kyori.Normalize();
			float angle = acosf(kyori.Dot(m_forward));
			if (fabsf(angle) < PI * ViewLenght) {
				m_asigaruState = Asigaru_tikazuki;
			}
		}
		//totugekiの処理
		m_moveSpeed = CVector3::Zero();
		CQuaternion ADDrot = CQuaternion::Identity();
		ADDrot.SetRotationDeg(CVector3::AxisY(), 2.0f);
		m_rotation.Multiply(ADDrot);
		break;
	}
		m_asigaruAnime.Play(m_asigaruState, 0.2f);
}

/// <summary>
/// 死んだときに呼ばれる名前の通りの関数
/// </summary>
void Enemy_asigaru::DeadMove()
{
	//当たり判定などの開放
	//m_ghostObject.Release();
	//プレイヤーから食らった技の威力に応じて
	//吹き飛ぶ量を決める
	float power;
	power = m_player->GetBlowOffPower();
	m_moveSpeed.y += power;
	CVector3 pushPower;
	pushPower = m_forward * power;
	m_moveSpeed -= pushPower;
	m_asigaruAnime.Play(Asigaru_dead,0.1f);
	//死んだアニメーションは早く再生する
	m_asigaruAnime.Update(1/60.0f);
}