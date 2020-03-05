#pragma once
#include "Player.h"
#include "Enemy.h"
/// <summary>
/// �Q�[���f�[�^�N���X
/// <remarks>
/// �Q�[���f�[�^�̃��X�g���ۑ������ق����ǂ����̂�ۑ�����N���X
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
	/// i�Ԗڂ̃X�|�b�g���擾����B
	/// </summary>
	CVector3 GetSpotPos(int i)
	{
		return EnemySpot[i];
	}
	/// <summary>
	/// i�Ԗڂ̃X�|�b�g���g���Ă��邩�H
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
	//�͂��̍ő吔
	static const int kakoi_max = 5;
	CVector3 EnemySpot[kakoi_max];
	Player* m_player;
	Enemy* m_enemy[kakoi_max];
	CVector3 CameraForword;
	CVector3 CameraRight;
	//�g�������͈͂̐��l
	const float kyori = 100.0f;
	const float half = 50.0f;
	//�ǂ̏ꏊ�̈͂����g���Ă��邩�B
	//enemy�^�̔z��ɂ��������s�������������B
	bool m_enclosure[kakoi_max] = { false,false,false,false,false };
};

