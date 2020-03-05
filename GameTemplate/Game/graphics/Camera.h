#pragma once
class Camera
{
public:
	/// <summary>
	/// �ˉe�s��̍X�V���@
	/// </summary>
	enum EnUpdateProjMatrixFunc {
		enUpdateProjMatrixFunc_Perspective,		//!<�����ˉe�s��B���ߖ@�������������Ȃ炱����
		enUpdateProjMatrixFunc_Ortho			//!<���s���e�B2D�I�ȕ\�����������Ƃ��͂�����
	};
	/*!
	 * @brief	�J�����s��A�v���W�F�N�V�����s��̍X�V�B
	 *@details
	 * ���ݐݒ肳��Ă���A�����_�A���_�A��p�Ȃǂ̏������Ƃ�
	 * �J�����s��ƃv���W�F�N�V�����s����X�V���܂��B
	 * ���̊֐����Ă΂Ȃ��ƁA�J�����͓����Ȃ��̂Œ��ӂ��K�v�ł��B
	 */
	void Update();
	/*!
	 * @brief	�J�����s����擾�B
	 */
	CMatrix GetViewMatrix()
	{
		return m_viewMatrix;
	}
	/*!
	 * @brief	�v���W�F�N�V�����s����擾�B
	 */
	CMatrix GetProjectionMatrix() 
	{
		return m_projMatrix;
	}
	/*!
	 * @brief	���_���擾�B
	 */
	CVector3 GetPosition()
	{
		return m_position;
	}
	/*!
	 * @brief	�����_���擾�B
	 */
	CVector3 GetTarget()
	{
		return m_target;
	}
	/*!
	 * @brief	�J�����̏�������擾�B
	 */
	CVector3 GetUp()
	{
		return m_up;
	}
	/*!
	 * @brief	�J�����̑O�������擾�B
	 */
	CVector3 GetForword()
	{
		return m_forward;
	}
	/*!
	 * @brief	�J�����̉��������擾�B
	 */
	CVector3 GetRight()
	{
		return m_right;
	}
	/*!
	 * @brief	���_��ݒ�B
	 */
	void SetPosition( CVector3 pos )
	{
		m_position = pos;
	}
	/*!
	 * @brief	�����_��ݒ�B
	 */
	void SetTarget( CVector3 target )
	{
		m_target = target;
	}
	/*!
	 * @brief	�J�����̏������ݒ�B
	 */
	void SetUp( CVector3 up )
	{
		m_up = up;
	}
	/*!
	 *   �J�����̑O������ݒ�B
	 */
	void SetForward(CVector3 forword)
	{
		m_forward = forword;
	}
	/*!
	 *   �J�����̉�������ݒ�B
	 */
	void SetRight(CVector3 right)
	{
		m_right = right;
	}
	/*!
	 * @brief	�����ʂ�ݒ�B
	 */
	void SetFar( float f )
	{
		m_far = f;
	}
	/*!
	 * @brief	�ߕ��ʂ�ݒ�B
	 */
	void SetNear( float n )
	{
		m_near = n;
	}
	/*!
	 * @brief	��p��ݒ�B
	 */
	void SetViewAngle( float angle )
	{
		m_viewAngle = angle;
	}
	/// <summary>
	/// ���s���e�̕���ݒ�
	/// </summary>
	/// <remarks>
	/// EnUpdateProjMatrixFunc��
	/// enUpdateProjMatrixFunc_Ortho�̎������g�p�����
	/// </remarks>
	/// <param name="h">��</param> 
	void SetWidth(float w)
	{
		m_width = w;
	}
	/// <summary>
/// ���s���e�̕���ݒ�
/// </summary>
/// <remarks>
/// EnUpdateProjMatrixFunc��
/// enUpdateProjMatrixFunc_Ortho�̎������g�p�����
	/// <param name="h">����</param>
	void SetHeight(float h)
	{
		m_height = h;
	}

	/// <summary>
	/// �ˉe�s��̌v�Z�̎d����ݒ�
	/// </summary>
	/// <param name="func">EnUpdateProjMatrixFunc���Q��</param>
	void SetUpdateProjMatrixFunc(EnUpdateProjMatrixFunc func)
	{
		m_updateProjeMatrixFunc = func;
	}
private:
	CMatrix	m_viewMatrix = CMatrix::Identity();		//�r���[�s��B
	CMatrix m_projMatrix = CMatrix::Identity();		//�v���W�F�N�V�����s��B
	CMatrix m_viewMatrixInv;                        //�r���[�s��
	CVector3 m_target = CVector3::Zero();			//�����_�B
	CVector3 m_position = CVector3::Zero();			//���_�B
	CVector3 m_up = CVector3::Up();					//������B
	CVector3 m_forward = CVector3::Front();         //�O�����B
	CVector3 m_right = CVector3::Right();           //�������B   
	float m_viewAngle = CMath::DegToRad(60.0f);		//��p�B
	float m_far = 10000.0f;							//�������ʂ܂ł̋����B
	float m_near = 1.0f;							//�ߕ��ʂ܂ł̋����B
	float m_width = 1280.0f;						//���s���e�̕��B
	float m_height = 720.0f;						//���s���e�̍���
	//�ˉe�s��̍쐬���@�B
	EnUpdateProjMatrixFunc m_updateProjeMatrixFunc = enUpdateProjMatrixFunc_Perspective;
};

extern Camera g_camera3D;		//!<3D�J�����B
extern Camera g_camera2D;		//!<2D�J�����B