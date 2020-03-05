#pragma once
#include "Player.h"
#include "Enemy.h"
/// <summary>
/// ゲームデータクラス
/// <remarks>
/// ゲームデータのリスト等保存したほうが良いものを保存するクラス
/// </remarks>
/// </summary>
class GameData : public IGameObject
{
public:
	GameData();
	~GameData();
	void Update() {};
	void Draw() {};
	void RandomPos();
	static GameData& GetInstans()
	{
		static GameData gamedata;
		return gamedata;
	}

	/// <summary>
	/// i番目のスポットを取得する。
	/// </summary>
	CVector3 GetSpotPos(int i)
	{
		return EnemySpot[i];
	}
	/// <summary>
	/// i番目のスポットが使われているか？
	/// </summary>
	/// <param name="i"></param>
	bool GetSpotUse(int i)
	{
		if(i < 0 || i > kakoi_max){
			return nullptr;
		}
		return m_enclosure[i];
	}
	void SetSpotUse(int i)
	{
		m_enclosure[i] = true;
	}

	void RemoveSpotUse(int i)
	{
		m_enclosure[i] = false;
	}

	int GetKakoiNum()
	{
		return kakoi_max;
	}
	void SetPlayerInfo(Player* pl)
	{
		m_player = pl;
	}
private:
	//囲いの最大数
	static const int kakoi_max = 5;
	CVector3 EnemySpot[kakoi_max];
	Player* m_player;
	Enemy* m_enemy[kakoi_max];
	CVector3 CameraForword;
	CVector3 CameraRight;
	//使いたい範囲の数値
	const float kyori = 100.0f;
	const float half = 50.0f;
	//どの場所の囲いが使われているか。
	//enemy型の配列にした方が都合がいいかも。
	bool m_enclosure[kakoi_max] = { false,false,false,false,false };
};

