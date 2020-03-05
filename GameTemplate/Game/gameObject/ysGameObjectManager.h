/*
  ysGameObject�̃}�l�[�W���[
*/
#pragma once
#include "gameObject/ysGameObject.h"
#include "util/Util.h"
	class ysGameObjectManager
	{
	public:
		ysGameObjectManager() {}
		~ysGameObjectManager() {}
		
		struct EffectTool
		{
			//Effekseer�}�l�[�W���Ǘ��B
			//�C���X�^���X��1��
			Effekseer::Manager* m_effekseerManager = nullptr;
			EffekseerRenderer::Renderer* m_effekseerRenderer = nullptr;

			//�G�t�F�N�g
			Effekseer::Effect* m_sampleEffect = nullptr;
			Effekseer::Handle m_playEffectHandle = -1;
		};

		void InitEffekseer();
		//�A�b�v�f�[�g
		void Update();
		/// <summary>
		/// �Q�[���I�u�W�F�N�g�̖��O�L�[���쐬
		/// </summary>
		static unsigned int MakeGameObjectNameKey(const char* objectName)
		{
			static const unsigned int defaultNameKey = Util::MakeHash("Undefined");
			unsigned int hash;
			if (objectName == nullptr) {
				hash = defaultNameKey;
			}
			else {
				hash = Util::MakeHash(objectName);
			}
			return hash;
		}

		//newGO
		template <class T>
		T* NewGameObject(const char* objectName)
		{
			T* newObj = new T;
			IGameObjectList.push_back(newObj);
			unsigned int hash = MakeGameObjectNameKey(objectName);
			newObj->m_nameKey = hash;
			return newObj;
		}

		//�f���[�g
		void DeleteGOObject(IGameObject* go)
		{
			//���X�g���猟�����āA����������폜����B
			for (auto it = IGameObjectList.begin();
				it != IGameObjectList.end();
				it++) {
				if ((*it) == go) {
					//�폜���N�G�X�g�𑗂�B
					go->RequestDelete();
				}
			}
		}

		/// <summary>
		/// �Q�[���I�u�W�F�N�g���̌����B�d��
		/// </summary>
		template<class T>
		T* FindGameObject(const char* objactName, bool enableErrorMessage)
		{
			unsigned int nameKey = Util::MakeHash(ObjectName);
			for (auto goList : IGameObjectList) {
				if (goList->m_nameKey == nameKey) {
					//����
					T* p = dynamic_cast<T*>(go);
					if (p == nullptr && enableErrorMessage == true) {
						//�^�ϊ��Ɏ��s
						MessageBox("�^�ϊ��Ɏ��s���܂���");
					}
					return p;
				}
			}
		}

		template<class T>
		void FindGameObjects(const char* objectName, std::function<bool(T* go)> func)
		{
			unsigned int nameKey = Util::MakeHash(objectName);
			for (auto goList : IGameObjectList) {
				if (goList->m_nameKey == nameKey) {
					//�������B
					T* p = dynamic_cast<T*>(goList);
					if (func(p) == false) {
						//�N�G�����f�B
						return;
					}
				}
			}
		}
		//�Ă΂ꂽ��1�ǉ�
		void Counting()
		{
			Count++;
		}
		int GetCount()
		{
			return Count;
		}
		//���s�BExecute����Draw��Update�ɕ���
		void Execute();
		//void Update();
		void Draw();
		//2d���ǉ�
		void Render();
	private:
		//�������B
		void Init(int gameObjectPropMax);

		void Start();
		//void Draw();
		void PostUpdate();
		void PostRender();

		void PreUpdate();
	private:
		//�z��̐����ϓ�������J�E���g����ϐ�����肽�������̂Œǉ�
		int Count = 0;
		
	private:
		//���ƔŊi�[��
		std::vector<IGameObject*> IGameObjectList; //�Q�[���I�u�W�F�N�g�̃��X�g
		//�z��̊i�[��
		typedef std::list<IGameObject*> GameObjectList;
		//List�͂��Ȃ�
		/// <summary>
		/// NewGO�p�̃��X�g�BtkEngine�ƈႢ�D��x�͂Ȃ��B
		/// </summary>
		std::vector<GameObjectList> m_gameObjectListArray;
		/// <summary>
		/// DeleteGO�p�̃��X�g
		/// </summary>
		std::vector<GameObjectList> m_deleteObjectArray[2];
	};
	//�O���A�N�Z�X������̂ŁAextern�錾������B
	extern ysGameObjectManager g_goMgr;
	/// <summary>
	/// �Q�[���I�u�W�F�N�g�����̃w���p�[�֐�
	/// </summary>
	template<class T>
	static inline T* NewGO(const char* objectName)
	{
		return ysGameObjectManager().NewGameObject<T>(objectName);
	}
	/// <summary>
	/// �Q�[���I�u�W�F�N�g�폜�̃w���p�[�֐�
	/// NewGO���g�p���č쐬�����I�u�W�F�N�g�͕K��DeleteGO�����s���邱��
	/// </summary>
	/// <param name="go">�폜����Q�[���I�u�W�F�N�g</param>
	static inline void DeleteGO(IGameObject* go)
	{
		g_goMgr.DeleteGOObject(go);
	}
	/// <summary>
	/// �Q�[���I�u�W�F�N�g�����̃w���p�[�֐�
	/// �����̃Q�[���I�u�W�F�N�g�S�Ăɑ΂���Query���s�������ꍇ�Ɏg�p����B
	/// objectName	�Q�[���I�u�W�F�N�g�̖��O
	/// func		�Q�[���I�u�W�F�N�g�������������ɌĂ΂��R�[���o�b�N�֐��B
	/// <param name="objectName"></param>
	/// <param name="func"></param>
	/// </summary>
	template<class T>
	static inline void QueryGOs(const char* objectName, std::function<bool(T* go)>func)
	{
		return g_goMgr.FindGameObjects<T>(objectName,func);
	}
	/// <summary>
	/// �Q�[���I�u�W�F�N�g�𖼑O�w��ō폜
	/// </summary>
	static inline void DeleteGOs(const char* objectName)
	{
		QueryGOs<IGameObject>(objectName, [](auto go) {
			DeleteGO(go);
			return true;
		});
	}
	extern ysGameObjectManager::EffectTool g_Effect;