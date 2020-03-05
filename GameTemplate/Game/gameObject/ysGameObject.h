#pragma once
/*
  �Q�[���I�u�W�F�N�g�̊��N���X�B
*/
	class ysGameObjectManager;
	class IGameObject{
	public:
		using IGameObjectIsBase = IGameObject;
		//�R���X�g���N�^
		IGameObject() :
			m_isStart(false),
			m_isDead(false)
		{}
		//�f�X�g���N�^
		virtual ~IGameObject()
		{

		}
		/// <summary>
			/// Update�̒��O�ŌĂ΂��J�n����
			/// </summary>
			/// <remarks>
			/// ���̊֐����Ă΂��ƃQ�[���I�u�W�F�N�g�̏��������������Ɣ��f����A
			/// Update�֐����Ă΂ꂾ���܂��Btrue��Ԃ��Ĉȍ~��start�֐��͌Ă΂�܂���B
			/// �Q�[���I�u�W�F�N�g�̏������ɕ����t���[��������ꍇ�Ȃǂ�false��Ԃ��āA�������X�e�b�v
			/// �Ȃǂ��g���ēK�؂ɏ��������Ă�������
			/// </remarks>
		virtual bool Start() { return true; }
		/// <summary>
		/// �X�V
		/// </summary>
		virtual void Update() {};
		/// <summary>
		/// Update�֐����Ă΂��O�ɌĂ΂��X�V����
		/// </summary>
		virtual void PostUpdate(){}
		/// <summary>
		/// Update�֐����Ă΂ꂽ��ɌĂ΂��X�V����
		/// </summary>
		virtual void PreUpdate(){}
		/// <summary>
		/// �`��
		/// </summary>
		virtual void Draw() {}
		/// <summary>
		/// �|�X�g�����_
		/// </summary>
		virtual void PostRender(){}
		/// <summary>
		/// �����_
		/// </summary>
		virtual void Render(){}
		/// <summary>
		/// deletego�����s���ꂽ�Ƃ��ɌĂ΂��
		/// </summary>
		virtual void OnDestroy() {};

		void StartWrapper()
		{
			if (m_isActive && !m_isStart && !m_isDead && !m_isRegist) {
				if (Start()) {
					//���������������������B
					m_isStart = true;
				}
			}
		}
		void UpdateWrapper()
		{
			if (m_isActive && m_isStart && !m_isDead && !m_isRegist) {
				Update();
			}
		}
		void PostUpdateWrapper()
		{
			if (m_isActive && m_isStart && !m_isDead && !m_isRegist) {
				PostUpdate();
			}
		}
		void PreUpdateWrapper()
		{
			if (m_isActive && m_isStart && !m_isDead && !m_isRegist) {
				PreUpdate();
			}
		}
		void DrawWrapper()
		{
			if (m_isActive && m_isStart && !m_isDead && !m_isRegist) {
				Draw();
			}
		}
		//�E���Ă��������֐��B�󗝂��邾���œ��e�����������킯�ł͂Ȃ��B
		void RequestDelete()
		{
			m_isRequestdelete = true;
		}
		//����ł邩�ǂ������m�F����֐��B
		bool isRequestDelete()
		{
			return m_isRequestdelete;
		}
		friend class ysGameObjectManager;
	private:
	protected:
		bool m_isStart;                  //Start�J�n�t���O
		bool m_isDead;                   //���S�t���O
		bool m_isRegistDeadList = false; //���S���X�g�ɐς܂�Ă���B
		bool m_isRegist = false;         //!<GameObjectManager>�ɓo�^����Ă��邩�ǂ���
		bool m_isActive = true;          //Active�t���O
		unsigned int m_nameKey = 0;		 //���O�L�[
		bool m_isRequestdelete = false;
	};