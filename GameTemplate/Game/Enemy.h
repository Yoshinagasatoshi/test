#pragma once
#include "gameObject/ysGameObject.h"
#include "Player.h"
/// <summary>
/// �G���N���X�B
/// <remarks>
/// �G�̊�{�I�ȓ�����^����N���X�ł�
/// ���y��G���N���X�̊��N���X�ɂȂ�\��
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
	SkinModel m_model;			//�X�L�����f���B
	SkinModel m_model_Row;		//���[�|���̃X�L�����f���B�A�j���͂��Ȃ��\��
	CVector3 m_position;		//���ݒn
	CQuaternion m_rotation;		//��]�p�x
	CVector3 m_scale;
	CVector3 m_playerPos;		//�v���C���[�̃|�W�V����
	Player* m_player;			//�v���C���[�̃|�C���g�^
	GameCamera* m_gameCamera;	//�Q�[���J�����̃|�C���g�^�B
	const float grabity = -9.8f * 2.0f;//�d��
	const float BattleRange = 180.0f * 180.0f;			//���̋����͈͓̔��ɋ߂Â��ƃo�g��
	const float VigilanceRange = 600.0f * 600.0f;		//���̋����͈͓̔��Ȃ�v���C���[�ɋ߂Â��B
};

